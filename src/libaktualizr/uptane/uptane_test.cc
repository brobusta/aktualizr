#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include "crypto/p11engine.h"
#include "httpfake.h"
#include "logging/logging.h"
#include "primary/initializer.h"
#include "primary/reportqueue.h"
#include "primary/sotauptaneclient.h"
#include "storage/fsstorage_read.h"
#include "storage/invstorage.h"
#include "test_utils.h"
#include "uptane/tuf.h"
#include "uptane/uptanerepository.h"
#include "utilities/utils.h"

#ifdef BUILD_P11
#ifndef TEST_PKCS11_MODULE_PATH
#define TEST_PKCS11_MODULE_PATH "/usr/local/softhsm/libsofthsm2.so"
#endif
#endif

boost::filesystem::path sysroot;

Uptane::SecondaryConfig addDefaultSecondary(Config& config, const TemporaryDirectory& temp_dir,
                                            const std::string& hw_id) {
  Uptane::SecondaryConfig ecu_config;
  ecu_config.secondary_type = Uptane::SecondaryType::kVirtual;
  ecu_config.partial_verifying = false;
  ecu_config.full_client_dir = temp_dir.Path();
  ecu_config.ecu_serial = "secondary_ecu_serial";
  ecu_config.ecu_hardware_id = hw_id;
  ecu_config.ecu_private_key = "sec.priv";
  ecu_config.ecu_public_key = "sec.pub";
  ecu_config.firmware_path = temp_dir / "firmware.txt";
  ecu_config.target_name_path = temp_dir / "firmware_name.txt";
  ecu_config.metadata_path = temp_dir / "secondary_metadata";
  config.uptane.secondary_configs.push_back(ecu_config);
  return ecu_config;
}

TEST(Uptane, Verify) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config config;
  config.uptane.director_server = http->tls_server + "/director";
  config.uptane.repo_server = http->tls_server + "/repo";

  config.storage.path = temp_dir.Path();
  auto storage = INvStorage::newStorage(config.storage);
  HttpResponse response = http->get(http->tls_server + "/director/root.json", HttpInterface::kNoLimit);
  Uptane::Root root(Uptane::Root::Policy::kAcceptAll);
  Uptane::Root(Uptane::RepositoryType::Director, response.getJson(), root);
}

TEST(Uptane, VerifyDataBad) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config config;
  config.uptane.director_server = http->tls_server + "/director";
  config.uptane.repo_server = http->tls_server + "/repo";

  config.storage.path = temp_dir.Path();
  auto storage = INvStorage::newStorage(config.storage);
  Json::Value data_json = http->get(http->tls_server + "/director/root.json", HttpInterface::kNoLimit).getJson();
  data_json.removeMember("signatures");

  Uptane::Root root(Uptane::Root::Policy::kAcceptAll);
  EXPECT_THROW(Uptane::Root(Uptane::RepositoryType::Director, data_json, root), Uptane::UnmetThreshold);
}

TEST(Uptane, VerifyDataUnknownType) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config config;
  config.uptane.director_server = http->tls_server + "/director";
  config.uptane.repo_server = http->tls_server + "/repo";

  config.storage.path = temp_dir.Path();
  auto storage = INvStorage::newStorage(config.storage);
  Json::Value data_json = http->get(http->tls_server + "/director/root.json", HttpInterface::kNoLimit).getJson();
  data_json["signatures"][0]["method"] = "badsignature";
  data_json["signatures"][1]["method"] = "badsignature";

  Uptane::Root root(Uptane::Root::Policy::kAcceptAll);
  EXPECT_THROW(Uptane::Root(Uptane::RepositoryType::Director, data_json, root), Uptane::SecurityException);
}

TEST(Uptane, VerifyDataBadKeyId) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config config;
  config.uptane.director_server = http->tls_server + "/director";
  config.uptane.repo_server = http->tls_server + "/repo";

  config.storage.path = temp_dir.Path();
  auto storage = INvStorage::newStorage(config.storage);
  Json::Value data_json = http->get(http->tls_server + "/director/root.json", HttpInterface::kNoLimit).getJson();

  data_json["signatures"][0]["keyid"] = "badkeyid";

  Uptane::Root root(Uptane::Root::Policy::kAcceptAll);
  EXPECT_THROW(Uptane::Root(Uptane::RepositoryType::Director, data_json, root), Uptane::BadKeyId);
}

TEST(Uptane, VerifyDataBadThreshold) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config config;
  config.uptane.director_server = http->tls_server + "/director";
  config.uptane.repo_server = http->tls_server + "/repo";

  config.storage.path = temp_dir.Path();
  auto storage = INvStorage::newStorage(config.storage);
  Json::Value data_json = http->get(http->tls_server + "/director/root.json", HttpInterface::kNoLimit).getJson();
  data_json["signed"]["roles"]["root"]["threshold"] = -1;
  try {
    Uptane::Root root(Uptane::Root::Policy::kAcceptAll);
    Uptane::Root(Uptane::RepositoryType::Director, data_json, root);
    FAIL();
  } catch (const Uptane::IllegalThreshold& ex) {
  } catch (const Uptane::UnmetThreshold& ex) {
  }
}

/*
 * \verify{\tst{153}} Check that aktualizr creates provisioning files if they
 * don't exist already.
 */
TEST(Uptane, Initialize) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config conf("tests/config/basic.toml");
  conf.uptane.repo_server = http->tls_server + "/director";

  conf.uptane.repo_server = http->tls_server + "/repo";
  conf.tls.server = http->tls_server;

  conf.storage.path = temp_dir.Path();
  conf.provision.primary_ecu_serial = "testecuserial";

  auto storage = INvStorage::newStorage(conf.storage);
  std::string pkey;
  std::string cert;
  std::string ca;
  bool result = storage->loadTlsCreds(&ca, &cert, &pkey);
  EXPECT_FALSE(result);

  KeyManager keys(storage, conf.keymanagerConfig());
  Initializer initializer(conf.provision, storage, http, keys, {});

  result = initializer.isSuccessful();
  EXPECT_TRUE(result);

  result = storage->loadTlsCreds(&ca, &cert, &pkey);
  EXPECT_TRUE(result);
  EXPECT_NE(ca, "");
  EXPECT_NE(cert, "");
  EXPECT_NE(pkey, "");

  Json::Value ecu_data = Utils::parseJSONFile(temp_dir.Path() / "post.json");
  EXPECT_EQ(ecu_data["ecus"].size(), 1);
  EXPECT_EQ(ecu_data["primary_ecu_serial"].asString(), conf.provision.primary_ecu_serial);
}

/*
 * \verify{\tst{154}} Check that aktualizr does NOT change provisioning files if
 * they DO exist already.
 */
TEST(Uptane, InitializeTwice) {
  TemporaryDirectory temp_dir;
  Config conf("tests/config/basic.toml");
  conf.storage.path = temp_dir.Path();
  conf.provision.primary_ecu_serial = "testecuserial";
  conf.storage.uptane_private_key_path = BasedPath("private.key");
  conf.storage.uptane_public_key_path = BasedPath("public.key");

  auto storage = INvStorage::newStorage(conf.storage);
  std::string pkey1;
  std::string cert1;
  std::string ca1;
  bool result = storage->loadTlsCreds(&ca1, &cert1, &pkey1);
  EXPECT_FALSE(result);

  auto http = std::make_shared<HttpFake>(temp_dir.Path());

  {
    KeyManager keys(storage, conf.keymanagerConfig());
    Initializer initializer(conf.provision, storage, http, keys, {});

    result = initializer.isSuccessful();
    EXPECT_TRUE(result);

    result = storage->loadTlsCreds(&ca1, &cert1, &pkey1);
    EXPECT_TRUE(result);
    EXPECT_NE(ca1, "");
    EXPECT_NE(cert1, "");
    EXPECT_NE(pkey1, "");
  }

  {
    KeyManager keys(storage, conf.keymanagerConfig());
    Initializer initializer(conf.provision, storage, http, keys, {});

    result = initializer.isSuccessful();
    EXPECT_TRUE(result);

    std::string pkey2;
    std::string cert2;
    std::string ca2;
    result = storage->loadTlsCreds(&ca2, &cert2, &pkey2);
    EXPECT_TRUE(result);

    EXPECT_EQ(cert1, cert2);
    EXPECT_EQ(ca1, ca2);
    EXPECT_EQ(pkey1, pkey2);
  }
}

/**
 * \verify{\tst{146}} Check that aktualizr does not generate a pet name when
 * device ID is specified. This is currently provisional and not a finalized
 * requirement at this time.
 */
TEST(Uptane, PetNameProvided) {
  TemporaryDirectory temp_dir;
  std::string test_name = "test-name-123";
  boost::filesystem::path device_path = temp_dir.Path() / "device_id";

  /* Make sure provided device ID is read as expected. */
  Config conf("tests/config/device_id.toml");
  conf.storage.path = temp_dir.Path();
  conf.storage.uptane_private_key_path = BasedPath("private.key");
  conf.storage.uptane_public_key_path = BasedPath("public.key");
  conf.provision.primary_ecu_serial = "testecuserial";

  auto storage = INvStorage::newStorage(conf.storage);
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  KeyManager keys(storage, conf.keymanagerConfig());
  Initializer initializer(conf.provision, storage, http, keys, {});

  EXPECT_TRUE(initializer.isSuccessful());

  {
    EXPECT_EQ(conf.provision.device_id, test_name);
    std::string devid;
    EXPECT_TRUE(storage->loadDeviceId(&devid));
    EXPECT_EQ(devid, test_name);
  }

  {
    /* Make sure name is unchanged after re-initializing config. */
    conf.postUpdateValues();
    EXPECT_EQ(conf.provision.device_id, test_name);
    std::string devid;
    EXPECT_TRUE(storage->loadDeviceId(&devid));
    EXPECT_EQ(devid, test_name);
  }
}

/**
 * \verify{\tst{145}} Check that aktualizr generates a pet name if no device ID
 * is specified.
 */
TEST(Uptane, PetNameCreation) {
  TemporaryDirectory temp_dir;
  boost::filesystem::path device_path = temp_dir.Path() / "device_id";

  // Make sure name is created.
  Config conf("tests/config/basic.toml");
  conf.storage.path = temp_dir.Path();
  conf.storage.uptane_private_key_path = BasedPath("private.key");
  conf.storage.uptane_public_key_path = BasedPath("public.key");
  conf.provision.primary_ecu_serial = "testecuserial";
  boost::filesystem::copy_file("tests/test_data/cred.zip", temp_dir.Path() / "cred.zip");
  conf.provision.provision_path = temp_dir.Path() / "cred.zip";

  std::string test_name1, test_name2;
  {
    auto storage = INvStorage::newStorage(conf.storage);
    auto http = std::make_shared<HttpFake>(temp_dir.Path());

    KeyManager keys(storage, conf.keymanagerConfig());
    Initializer initializer(conf.provision, storage, http, keys, {});

    EXPECT_TRUE(initializer.isSuccessful());

    EXPECT_TRUE(storage->loadDeviceId(&test_name1));
    EXPECT_NE(test_name1, "");
  }

  // Make sure a new name is generated if the config does not specify a name and
  // there is no device_id file.
  TemporaryDirectory temp_dir2;
  {
    conf.storage.path = temp_dir2.Path();
    boost::filesystem::copy_file("tests/test_data/cred.zip", temp_dir2.Path() / "cred.zip");
    conf.provision.device_id = "";

    auto storage = INvStorage::newStorage(conf.storage);
    auto http = std::make_shared<HttpFake>(temp_dir.Path());
    KeyManager keys(storage, conf.keymanagerConfig());
    Initializer initializer(conf.provision, storage, http, keys, {});

    EXPECT_TRUE(initializer.isSuccessful());

    EXPECT_TRUE(storage->loadDeviceId(&test_name2));
    EXPECT_NE(test_name2, test_name1);
  }

  // If the device_id is cleared in the config, but the file is still present,
  // re-initializing the config should still read the device_id from file.
  {
    conf.provision.device_id = "";
    auto storage = INvStorage::newStorage(conf.storage);
    auto http = std::make_shared<HttpFake>(temp_dir.Path());
    KeyManager keys(storage, conf.keymanagerConfig());
    Initializer initializer(conf.provision, storage, http, keys, {});

    EXPECT_TRUE(initializer.isSuccessful());

    std::string devid;
    EXPECT_TRUE(storage->loadDeviceId(&devid));
    EXPECT_EQ(devid, test_name2);
  }

  // If the device_id file is removed, but the field is still present in the
  // config, re-initializing the config should still read the device_id from
  // config.
  {
    TemporaryDirectory temp_dir3;
    conf.storage.path = temp_dir3.Path();
    boost::filesystem::copy_file("tests/test_data/cred.zip", temp_dir3.Path() / "cred.zip");
    conf.provision.device_id = test_name2;

    auto storage = INvStorage::newStorage(conf.storage);
    auto http = std::make_shared<HttpFake>(temp_dir.Path());
    KeyManager keys(storage, conf.keymanagerConfig());
    Initializer initializer(conf.provision, storage, http, keys, {});

    EXPECT_TRUE(initializer.isSuccessful());

    std::string devid;
    EXPECT_TRUE(storage->loadDeviceId(&devid));
    EXPECT_EQ(devid, test_name2);
  }
}

TEST(Uptane, InitializeFail) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config conf("tests/config/basic.toml");
  conf.uptane.repo_server = http->tls_server + "/director";
  conf.storage.path = temp_dir.Path();
  conf.storage.uptane_private_key_path = BasedPath("private.key");
  conf.storage.uptane_public_key_path = BasedPath("public.key");

  conf.uptane.repo_server = http->tls_server + "/repo";
  conf.tls.server = http->tls_server;

  conf.provision.primary_ecu_serial = "testecuserial";

  auto storage = INvStorage::newStorage(conf.storage);

  http->provisioningResponse = ProvisioningResult::kFailure;
  KeyManager keys(storage, conf.keymanagerConfig());
  Initializer initializer(conf.provision, storage, http, keys, {});

  bool result = initializer.isSuccessful();
  http->provisioningResponse = ProvisioningResult::kOK;
  EXPECT_FALSE(result);
}

TEST(Uptane, AssembleManifestGood) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config config;
  config.storage.path = temp_dir.Path();
  config.storage.uptane_metadata_path = BasedPath("metadata");
  config.storage.uptane_private_key_path = BasedPath("private.key");
  config.storage.uptane_public_key_path = BasedPath("public.key");
  boost::filesystem::copy_file("tests/test_data/cred.zip", (temp_dir / "cred.zip").string());
  boost::filesystem::copy_file("tests/test_data/firmware.txt", (temp_dir / "firmware.txt").string());
  boost::filesystem::copy_file("tests/test_data/firmware_name.txt", (temp_dir / "firmware_name.txt").string());
  config.provision.provision_path = temp_dir / "cred.zip";
  config.provision.mode = ProvisionMode::kAutomatic;
  config.uptane.director_server = http->tls_server + "/director";
  config.uptane.repo_server = http->tls_server + "/repo";
  config.provision.primary_ecu_serial = "testecuserial";
  config.pacman.type = PackageManager::kNone;
  addDefaultSecondary(config, temp_dir, "secondary_hardware");

  auto storage = INvStorage::newStorage(config.storage);
  auto sota_client = SotaUptaneClient::newTestClient(config, storage, http);
  EXPECT_TRUE(sota_client->initialize());

  Json::Value manifest = sota_client->AssembleManifest();
  EXPECT_EQ(manifest.size(), 2);
}

TEST(Uptane, AssembleManifestBad) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config config;
  config.storage.path = temp_dir.Path();
  config.storage.uptane_metadata_path = BasedPath("metadata");
  config.storage.uptane_private_key_path = BasedPath("private.key");
  config.storage.uptane_public_key_path = BasedPath("public.key");
  boost::filesystem::copy_file("tests/test_data/cred.zip", (temp_dir / "cred.zip").string());
  boost::filesystem::copy_file("tests/test_data/firmware.txt", (temp_dir / "firmware.txt").string());
  boost::filesystem::copy_file("tests/test_data/firmware_name.txt", (temp_dir / "firmware_name.txt").string());
  config.provision.provision_path = temp_dir / "cred.zip";
  config.provision.mode = ProvisionMode::kAutomatic;
  config.uptane.director_server = http->tls_server + "/director";
  config.uptane.repo_server = http->tls_server + "/repo";
  config.provision.primary_ecu_serial = "testecuserial";
  config.pacman.type = PackageManager::kNone;
  Uptane::SecondaryConfig ecu_config = addDefaultSecondary(config, temp_dir, "secondary_hardware");

  std::string private_key, public_key;
  Crypto::generateKeyPair(ecu_config.key_type, &public_key, &private_key);
  Utils::writeFile(ecu_config.full_client_dir / ecu_config.ecu_private_key, private_key);
  public_key = Utils::readFile("tests/test_data/public.key");
  Utils::writeFile(ecu_config.full_client_dir / ecu_config.ecu_public_key, public_key);

  auto storage = INvStorage::newStorage(config.storage);

  auto sota_client = SotaUptaneClient::newTestClient(config, storage, http);
  EXPECT_TRUE(sota_client->initialize());

  Json::Value manifest = sota_client->AssembleManifest();

  EXPECT_EQ(manifest.size(), 1);
  EXPECT_EQ(manifest["testecuserial"]["signed"]["ecu_serial"].asString(), config.provision.primary_ecu_serial);
}

TEST(Uptane, PutManifest) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config config;
  config.storage.path = temp_dir.Path();
  config.storage.uptane_metadata_path = BasedPath("metadata");
  config.storage.uptane_private_key_path = BasedPath("private.key");
  config.storage.uptane_public_key_path = BasedPath("public.key");
  boost::filesystem::copy_file("tests/test_data/cred.zip", (temp_dir / "cred.zip").string());
  boost::filesystem::copy_file("tests/test_data/firmware.txt", (temp_dir / "firmware.txt").string());
  boost::filesystem::copy_file("tests/test_data/firmware_name.txt", (temp_dir / "firmware_name.txt").string());
  config.provision.provision_path = temp_dir / "cred.zip";
  config.provision.mode = ProvisionMode::kAutomatic;
  config.uptane.director_server = http->tls_server + "/director";
  config.uptane.repo_server = http->tls_server + "/repo";
  config.provision.primary_ecu_serial = "testecuserial";
  config.pacman.type = PackageManager::kNone;
  addDefaultSecondary(config, temp_dir, "secondary_hardware");

  auto storage = INvStorage::newStorage(config.storage);

  std::shared_ptr<event::Channel> events_channel{new event::Channel};
  auto sota_client = SotaUptaneClient::newTestClient(config, storage, http, events_channel);
  EXPECT_TRUE(sota_client->initialize());

  std::shared_ptr<command::Channel> commands_channel{new command::Channel};

  *commands_channel << std::make_shared<command::PutManifest>();
  *commands_channel << std::make_shared<command::Shutdown>();
  sota_client->runForever(commands_channel);

  EXPECT_TRUE(boost::filesystem::exists(temp_dir / http->test_manifest));
  Json::Value json = Utils::parseJSONFile((temp_dir / http->test_manifest).string());

  EXPECT_EQ(json["signatures"].size(), 1u);
  EXPECT_EQ(json["signed"]["primary_ecu_serial"].asString(), "testecuserial");
  EXPECT_EQ(
      json["signed"]["ecu_version_manifests"]["testecuserial"]["signed"]["installed_image"]["filepath"].asString(),
      "unknown");
  EXPECT_EQ(json["signed"]["ecu_version_manifests"].size(), 2u);
  EXPECT_EQ(json["signed"]["ecu_version_manifests"]["secondary_ecu_serial"]["signed"]["ecu_serial"].asString(),
            "secondary_ecu_serial");
  EXPECT_EQ(json["signed"]["ecu_version_manifests"]["secondary_ecu_serial"]["signed"]["installed_image"]["filepath"]
                .asString(),
            "test-package");
}

TEST(Uptane, RunForeverNoUpdates) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config conf("tests/config/basic.toml");
  boost::filesystem::copy_file("tests/test_data/secondary_firmware.txt", temp_dir / "secondary_firmware.txt");
  conf.uptane.director_server = http->tls_server + "/director";
  conf.uptane.repo_server = http->tls_server + "/repo";
  conf.provision.primary_ecu_serial = "CA:FE:A6:D2:84:9D";
  conf.uptane.running_mode = RunningMode::kFull;
  conf.provision.primary_ecu_hardware_id = "primary_hw";
  conf.storage.path = temp_dir.Path();
  conf.storage.uptane_metadata_path = BasedPath("metadata");
  conf.storage.uptane_private_key_path = BasedPath("private.key");
  conf.storage.uptane_public_key_path = BasedPath("public.key");
  conf.pacman.sysroot = sysroot;
  conf.tls.server = http->tls_server;
  addDefaultSecondary(conf, temp_dir, "secondary_hw");

  std::shared_ptr<event::Channel> events_channel{new event::Channel};
  std::shared_ptr<command::Channel> commands_channel{new command::Channel};

  *commands_channel << std::make_shared<command::FetchMeta>();
  *commands_channel << std::make_shared<command::CheckUpdates>();
  *commands_channel << std::make_shared<command::FetchMeta>();
  *commands_channel << std::make_shared<command::CheckUpdates>();
  *commands_channel << std::make_shared<command::Shutdown>();

  auto storage = INvStorage::newStorage(conf.storage);

  auto up = SotaUptaneClient::newTestClient(conf, storage, http, events_channel);
  up->runForever(commands_channel);

  std::shared_ptr<event::BaseEvent> event;

  events_channel->setTimeout(std::chrono::milliseconds(1000));
  EXPECT_TRUE(events_channel->hasValues());
  EXPECT_TRUE(*events_channel >> event);
  EXPECT_EQ(event->variant, "FetchMetaComplete");

  EXPECT_TRUE(events_channel->hasValues());
  EXPECT_TRUE(*events_channel >> event);
  EXPECT_EQ(event->variant, "UpdateAvailable");

  EXPECT_TRUE(events_channel->hasValues());
  EXPECT_TRUE(*events_channel >> event);
  EXPECT_EQ(event->variant, "FetchMetaComplete");

  EXPECT_TRUE(events_channel->hasValues());
  EXPECT_TRUE(*events_channel >> event);
  EXPECT_EQ(event->variant, "UptaneTimestampUpdated");

  Json::Value manifest = up->AssembleManifest();
  EXPECT_FALSE(manifest["testecuserial"]["signed"].isMember("custom"));
}

TEST(Uptane, RunForeverHasUpdates) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config conf("tests/config/basic.toml");
  boost::filesystem::copy_file("tests/test_data/secondary_firmware.txt", temp_dir / "secondary_firmware.txt");
  conf.uptane.director_server = http->tls_server + "/director";
  conf.uptane.repo_server = http->tls_server + "/repo";
  conf.provision.primary_ecu_serial = "CA:FE:A6:D2:84:9D";
  conf.provision.primary_ecu_hardware_id = "primary_hw";
  conf.uptane.polling_sec = 1;
  conf.storage.path = temp_dir.Path();
  conf.storage.uptane_metadata_path = BasedPath("metadata");
  conf.storage.uptane_private_key_path = BasedPath("private.key");
  conf.storage.uptane_public_key_path = BasedPath("public.key");
  conf.pacman.sysroot = sysroot;
  conf.tls.server = http->tls_server;
  addDefaultSecondary(conf, temp_dir, "secondary_hw");

  std::shared_ptr<event::Channel> events_channel{new event::Channel};
  std::shared_ptr<command::Channel> commands_channel{new command::Channel};

  *commands_channel << std::make_shared<command::FetchMeta>();
  *commands_channel << std::make_shared<command::CheckUpdates>();
  *commands_channel << std::make_shared<command::Shutdown>();
  auto storage = INvStorage::newStorage(conf.storage);
  auto up = SotaUptaneClient::newTestClient(conf, storage, http, events_channel);
  up->runForever(commands_channel);

  std::shared_ptr<event::BaseEvent> event;
  EXPECT_TRUE(events_channel->hasValues());
  EXPECT_TRUE(*events_channel >> event);
  EXPECT_EQ(event->variant, "FetchMetaComplete");
  EXPECT_TRUE(*events_channel >> event);
  EXPECT_EQ(event->variant, "UpdateAvailable");
  auto targets_event = dynamic_cast<event::UpdateAvailable*>(event.get());
  EXPECT_EQ(targets_event->updates.size(), 2u);
  EXPECT_EQ(targets_event->updates[0].filename(), "primary_firmware.txt");
  EXPECT_EQ(targets_event->updates[1].filename(), "secondary_firmware.txt");

  Json::Value manifest = up->AssembleManifest();
  EXPECT_FALSE(manifest["testecuserial"]["signed"].isMember("custom"));
}

std::vector<Uptane::Target> makePackage(const std::string& serial, const std::string& hw_id) {
  std::vector<Uptane::Target> packages_to_install;
  Json::Value ot_json;
  ot_json["custom"]["ecuIdentifiers"][serial]["hardwareId"] = hw_id;
  ot_json["custom"]["targetFormat"] = "OSTREE";
  ot_json["length"] = 0;
  ot_json["hashes"]["sha256"] = serial;
  packages_to_install.emplace_back(serial, ot_json);
  return packages_to_install;
}

TEST(Uptane, RunForeverInstall) {
  Config conf("tests/config/basic.toml");
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  conf.provision.primary_ecu_serial = "testecuserial";
  conf.provision.primary_ecu_hardware_id = "testecuhwid";
  conf.uptane.director_server = http->tls_server + "/director";
  conf.uptane.repo_server = http->tls_server + "/repo";
  conf.uptane.polling_sec = 1;
  conf.storage.path = temp_dir.Path();
  conf.storage.uptane_private_key_path = BasedPath("private.key");
  conf.storage.uptane_public_key_path = BasedPath("public.key");
  conf.pacman.sysroot = sysroot;

  conf.tls.server = http->tls_server;
  std::shared_ptr<event::Channel> events_channel{new event::Channel};
  std::shared_ptr<command::Channel> commands_channel{new command::Channel};

  std::vector<Uptane::Target> packages_to_install = makePackage("testecuserial", "testecuhwid");
  *commands_channel << std::make_shared<command::UptaneInstall>(packages_to_install);
  *commands_channel << std::make_shared<command::Shutdown>();
  auto storage = INvStorage::newStorage(conf.storage);

  auto up = SotaUptaneClient::newTestClient(conf, storage, http, events_channel);
  up->runForever(commands_channel);

  EXPECT_FALSE(boost::filesystem::exists(temp_dir.Path() / http->test_manifest));

  // Make sure operation_result and filepath were correctly written and formatted.
  Json::Value manifest = up->AssembleManifest();
  EXPECT_EQ(manifest["testecuserial"]["signed"]["custom"]["operation_result"]["id"].asString(), "testecuserial");
  EXPECT_EQ(manifest["testecuserial"]["signed"]["custom"]["operation_result"]["result_code"].asInt(),
            static_cast<int>(data::UpdateResultCode::kOk));
  EXPECT_EQ(manifest["testecuserial"]["signed"]["custom"]["operation_result"]["result_text"].asString(),
            "Installing fake package was successful");
  EXPECT_EQ(manifest["testecuserial"]["signed"]["installed_image"]["filepath"].asString(), "testecuserial");
}

TEST(Uptane, UptaneSecondaryAdd) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config config;
  config.uptane.repo_server = http->tls_server + "/director";
  boost::filesystem::copy_file("tests/test_data/cred.zip", temp_dir / "cred.zip");
  config.provision.provision_path = temp_dir / "cred.zip";
  config.provision.mode = ProvisionMode::kAutomatic;
  config.uptane.repo_server = http->tls_server + "/repo";
  config.tls.server = http->tls_server;
  config.provision.primary_ecu_serial = "testecuserial";
  config.storage.path = temp_dir.Path();
  config.storage.uptane_private_key_path = BasedPath("private.key");
  config.storage.uptane_public_key_path = BasedPath("public.key");
  config.pacman.type = PackageManager::kNone;
  addDefaultSecondary(config, temp_dir, "secondary_hardware");

  auto storage = INvStorage::newStorage(config.storage);
  auto sota_client = SotaUptaneClient::newTestClient(config, storage, http);
  EXPECT_TRUE(sota_client->initialize());
  Json::Value ecu_data = Utils::parseJSONFile(temp_dir / "post.json");
  EXPECT_EQ(ecu_data["ecus"].size(), 2);
  EXPECT_EQ(ecu_data["primary_ecu_serial"].asString(), config.provision.primary_ecu_serial);
  EXPECT_EQ(ecu_data["ecus"][1]["ecu_serial"].asString(), "secondary_ecu_serial");
  EXPECT_EQ(ecu_data["ecus"][1]["hardware_identifier"].asString(), "secondary_hardware");
  EXPECT_EQ(ecu_data["ecus"][1]["clientKey"]["keytype"].asString(), "RSA");
  EXPECT_TRUE(ecu_data["ecus"][1]["clientKey"]["keyval"]["public"].asString().size() > 0);
}

/**
 * \verify{\tst{149}} Check that basic device info sent by aktualizr on provisioning are on server
 * Also test that installation works as expected with the fake package manager.
 */
TEST(Uptane, ProvisionOnServer) {
  TemporaryDirectory temp_dir;
  Config config("tests/config/basic.toml");
  std::string server = "tst149";
  config.provision.server = server;
  config.tls.server = server;
  config.uptane.director_server = server + "/director";
  config.uptane.repo_server = server + "/repo";
  config.provision.device_id = "tst149_device_id";
  config.provision.primary_ecu_hardware_id = "tst149_hardware_identifier";
  config.provision.primary_ecu_serial = "tst149_ecu_serial";
  config.uptane.polling_sec = 1;
  config.storage.path = temp_dir.Path();

  std::shared_ptr<event::Channel> events_channel{new event::Channel};
  std::shared_ptr<command::Channel> commands_channel{new command::Channel};
  auto storage = INvStorage::newStorage(config.storage);
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  std::vector<Uptane::Target> packages_to_install =
      makePackage(config.provision.primary_ecu_serial, config.provision.primary_ecu_hardware_id);
  *commands_channel << std::make_shared<command::FetchMeta>();
  *commands_channel << std::make_shared<command::StartDownload>(packages_to_install);
  *commands_channel << std::make_shared<command::UptaneInstall>(packages_to_install);
  *commands_channel << std::make_shared<command::Shutdown>();

  auto up = SotaUptaneClient::newTestClient(config, storage, http, events_channel);
  up->runForever(commands_channel);
}

TEST(Uptane, CheckOldProvision) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  int result = system((std::string("cp -rf tests/test_data/oldprovdir/* ") + temp_dir.PathString()).c_str());
  (void)result;
  Config config;
  config.tls.server = http->tls_server;
  config.uptane.director_server = http->tls_server + "/director";
  config.uptane.repo_server = http->tls_server + "/repo";
  config.storage.path = temp_dir.Path();

  // note: trigger migration from fs to sql
  auto storage = INvStorage::newStorage(config.storage);
  EXPECT_FALSE(storage->loadEcuRegistered());

  KeyManager keys(storage, config.keymanagerConfig());
  Initializer initializer(config.provision, storage, http, keys, {});

  EXPECT_TRUE(initializer.isSuccessful());
  EXPECT_TRUE(storage->loadEcuRegistered());
}

TEST(Uptane, fs_to_sql_full) {
  TemporaryDirectory temp_dir;
  int result = system((std::string("cp -rf tests/test_data/prov/* ") + temp_dir.PathString()).c_str());
  (void)result;
  StorageConfig config;
  config.type = StorageType::kSqlite;
  config.uptane_metadata_path = BasedPath("metadata");
  config.path = temp_dir.Path();

  config.uptane_private_key_path = BasedPath("ecukey.der");
  config.tls_cacert_path = BasedPath("root.crt");

  FSStorageRead fs_storage(config);

  std::string public_key;
  std::string private_key;
  fs_storage.loadPrimaryKeys(&public_key, &private_key);

  std::string ca;
  std::string cert;
  std::string pkey;
  fs_storage.loadTlsCreds(&ca, &cert, &pkey);

  std::string device_id;
  fs_storage.loadDeviceId(&device_id);

  EcuSerials serials;
  fs_storage.loadEcuSerials(&serials);

  bool ecu_registered = fs_storage.loadEcuRegistered() ? true : false;

  std::vector<Uptane::Target> installed_versions;
  fs_storage.loadInstalledVersions(&installed_versions);

  std::string director_root;
  std::string director_targets;
  std::string images_root;
  std::string images_targets;
  std::string images_timestamp;
  std::string images_snapshot;

  EXPECT_TRUE(fs_storage.loadLatestRoot(&director_root, Uptane::RepositoryType::Director));
  EXPECT_TRUE(fs_storage.loadNonRoot(&director_targets, Uptane::RepositoryType::Director, Uptane::Role::Targets()));
  EXPECT_TRUE(fs_storage.loadLatestRoot(&images_root, Uptane::RepositoryType::Images));
  EXPECT_TRUE(fs_storage.loadNonRoot(&images_targets, Uptane::RepositoryType::Images, Uptane::Role::Targets()));
  EXPECT_TRUE(fs_storage.loadNonRoot(&images_timestamp, Uptane::RepositoryType::Images, Uptane::Role::Timestamp()));
  EXPECT_TRUE(fs_storage.loadNonRoot(&images_snapshot, Uptane::RepositoryType::Images, Uptane::Role::Snapshot()));

  EXPECT_TRUE(boost::filesystem::exists(config.uptane_public_key_path.get(config.path)));
  EXPECT_TRUE(boost::filesystem::exists(config.uptane_private_key_path.get(config.path)));
  EXPECT_TRUE(boost::filesystem::exists(config.tls_cacert_path.get(config.path)));
  EXPECT_TRUE(boost::filesystem::exists(config.tls_clientcert_path.get(config.path)));
  EXPECT_TRUE(boost::filesystem::exists(config.tls_pkey_path.get(config.path)));

  boost::filesystem::path image_path = config.uptane_metadata_path.get(config.path) / "repo";
  boost::filesystem::path director_path = config.uptane_metadata_path.get(config.path) / "director";
  EXPECT_TRUE(boost::filesystem::exists(director_path / "1.root.json"));
  EXPECT_TRUE(boost::filesystem::exists(director_path / "targets.json"));
  EXPECT_TRUE(boost::filesystem::exists(image_path / "1.root.json"));
  EXPECT_TRUE(boost::filesystem::exists(image_path / "targets.json"));
  EXPECT_TRUE(boost::filesystem::exists(image_path / "timestamp.json"));
  EXPECT_TRUE(boost::filesystem::exists(image_path / "snapshot.json"));
  EXPECT_TRUE(boost::filesystem::exists(Utils::absolutePath(config.path, "device_id")));
  EXPECT_TRUE(boost::filesystem::exists(Utils::absolutePath(config.path, "is_registered")));
  EXPECT_TRUE(boost::filesystem::exists(Utils::absolutePath(config.path, "primary_ecu_serial")));
  EXPECT_TRUE(boost::filesystem::exists(Utils::absolutePath(config.path, "primary_ecu_hardware_id")));
  EXPECT_TRUE(boost::filesystem::exists(Utils::absolutePath(config.path, "secondaries_list")));
  auto sql_storage = INvStorage::newStorage(config);

  EXPECT_FALSE(boost::filesystem::exists(config.uptane_public_key_path.get(config.path)));
  EXPECT_FALSE(boost::filesystem::exists(config.uptane_private_key_path.get(config.path)));
  EXPECT_FALSE(boost::filesystem::exists(config.tls_cacert_path.get(config.path)));
  EXPECT_FALSE(boost::filesystem::exists(config.tls_clientcert_path.get(config.path)));
  EXPECT_FALSE(boost::filesystem::exists(config.tls_pkey_path.get(config.path)));

  EXPECT_FALSE(boost::filesystem::exists(director_path / "root.json"));
  EXPECT_FALSE(boost::filesystem::exists(director_path / "targets.json"));
  EXPECT_FALSE(boost::filesystem::exists(director_path / "root.json"));
  EXPECT_FALSE(boost::filesystem::exists(director_path / "targets.json"));
  EXPECT_FALSE(boost::filesystem::exists(image_path / "timestamp.json"));
  EXPECT_FALSE(boost::filesystem::exists(image_path / "snapshot.json"));
  EXPECT_FALSE(boost::filesystem::exists(Utils::absolutePath(config.path, "device_id")));
  EXPECT_FALSE(boost::filesystem::exists(Utils::absolutePath(config.path, "is_registered")));
  EXPECT_FALSE(boost::filesystem::exists(Utils::absolutePath(config.path, "primary_ecu_serial")));
  EXPECT_FALSE(boost::filesystem::exists(Utils::absolutePath(config.path, "primary_ecu_hardware_id")));
  EXPECT_FALSE(boost::filesystem::exists(Utils::absolutePath(config.path, "secondaries_list")));

  std::string sql_public_key;
  std::string sql_private_key;
  sql_storage->loadPrimaryKeys(&sql_public_key, &sql_private_key);

  std::string sql_ca;
  std::string sql_cert;
  std::string sql_pkey;
  sql_storage->loadTlsCreds(&sql_ca, &sql_cert, &sql_pkey);

  std::string sql_device_id;
  sql_storage->loadDeviceId(&sql_device_id);

  EcuSerials sql_serials;
  sql_storage->loadEcuSerials(&sql_serials);

  bool sql_ecu_registered = sql_storage->loadEcuRegistered() ? true : false;

  std::vector<Uptane::Target> sql_installed_versions;
  sql_storage->loadInstalledVersions(&sql_installed_versions);

  std::string sql_director_root;
  std::string sql_director_targets;
  std::string sql_images_root;
  std::string sql_images_targets;
  std::string sql_images_timestamp;
  std::string sql_images_snapshot;

  sql_storage->loadLatestRoot(&sql_director_root, Uptane::RepositoryType::Director);
  sql_storage->loadNonRoot(&sql_director_targets, Uptane::RepositoryType::Director, Uptane::Role::Targets());
  sql_storage->loadLatestRoot(&sql_images_root, Uptane::RepositoryType::Images);
  sql_storage->loadNonRoot(&sql_images_targets, Uptane::RepositoryType::Images, Uptane::Role::Targets());
  sql_storage->loadNonRoot(&sql_images_timestamp, Uptane::RepositoryType::Images, Uptane::Role::Timestamp());
  sql_storage->loadNonRoot(&sql_images_snapshot, Uptane::RepositoryType::Images, Uptane::Role::Snapshot());

  EXPECT_EQ(sql_public_key, public_key);
  EXPECT_EQ(sql_private_key, private_key);
  EXPECT_EQ(sql_ca, ca);
  EXPECT_EQ(sql_cert, cert);
  EXPECT_EQ(sql_pkey, pkey);
  EXPECT_EQ(sql_device_id, device_id);
  EXPECT_EQ(sql_serials, serials);
  EXPECT_EQ(sql_ecu_registered, ecu_registered);
  EXPECT_EQ(sql_installed_versions, installed_versions);

  EXPECT_EQ(sql_director_root, director_root);
  EXPECT_EQ(sql_director_targets, director_targets);
  EXPECT_EQ(sql_images_root, images_root);
  EXPECT_EQ(sql_images_targets, images_targets);
  EXPECT_EQ(sql_images_timestamp, images_timestamp);
  EXPECT_EQ(sql_images_snapshot, images_snapshot);
}

TEST(Uptane, fs_to_sql_partial) {
  TemporaryDirectory temp_dir;
  boost::filesystem::copy_file("tests/test_data/prov/ecukey.der", temp_dir.Path() / "ecukey.der");
  boost::filesystem::copy_file("tests/test_data/prov/ecukey.pub", temp_dir.Path() / "ecukey.pub");

  StorageConfig config;
  config.type = StorageType::kSqlite;
  config.uptane_metadata_path = BasedPath("metadata");
  config.path = temp_dir.Path();

  config.uptane_private_key_path = BasedPath("ecukey.der");
  config.tls_cacert_path = BasedPath("root.crt");

  FSStorageRead fs_storage(config);

  std::string public_key;
  std::string private_key;
  fs_storage.loadPrimaryKeys(&public_key, &private_key);

  std::string ca;
  std::string cert;
  std::string pkey;
  fs_storage.loadTlsCreds(&ca, &cert, &pkey);

  std::string device_id;
  fs_storage.loadDeviceId(&device_id);

  EcuSerials serials;
  fs_storage.loadEcuSerials(&serials);

  bool ecu_registered = fs_storage.loadEcuRegistered() ? true : false;

  std::vector<Uptane::Target> installed_versions;
  fs_storage.loadInstalledVersions(&installed_versions);

  std::string director_root;
  std::string director_targets;
  std::string images_root;
  std::string images_targets;
  std::string images_timestamp;
  std::string images_snapshot;

  fs_storage.loadLatestRoot(&director_root, Uptane::RepositoryType::Director);
  fs_storage.loadNonRoot(&director_targets, Uptane::RepositoryType::Director, Uptane::Role::Targets());
  fs_storage.loadLatestRoot(&images_root, Uptane::RepositoryType::Images);
  fs_storage.loadNonRoot(&images_targets, Uptane::RepositoryType::Images, Uptane::Role::Targets());
  fs_storage.loadNonRoot(&images_timestamp, Uptane::RepositoryType::Images, Uptane::Role::Timestamp());
  fs_storage.loadNonRoot(&images_snapshot, Uptane::RepositoryType::Images, Uptane::Role::Snapshot());

  EXPECT_TRUE(boost::filesystem::exists(config.uptane_public_key_path.get(config.path)));
  EXPECT_TRUE(boost::filesystem::exists(config.uptane_private_key_path.get(config.path)));

  auto sql_storage = INvStorage::newStorage(config);

  EXPECT_FALSE(boost::filesystem::exists(config.uptane_public_key_path.get(config.path)));
  EXPECT_FALSE(boost::filesystem::exists(config.uptane_private_key_path.get(config.path)));

  std::string sql_public_key;
  std::string sql_private_key;
  sql_storage->loadPrimaryKeys(&sql_public_key, &sql_private_key);

  std::string sql_ca;
  std::string sql_cert;
  std::string sql_pkey;
  sql_storage->loadTlsCreds(&sql_ca, &sql_cert, &sql_pkey);

  std::string sql_device_id;
  sql_storage->loadDeviceId(&sql_device_id);

  EcuSerials sql_serials;
  sql_storage->loadEcuSerials(&sql_serials);

  bool sql_ecu_registered = sql_storage->loadEcuRegistered() ? true : false;

  std::vector<Uptane::Target> sql_installed_versions;
  sql_storage->loadInstalledVersions(&sql_installed_versions);

  std::string sql_director_root;
  std::string sql_director_targets;
  std::string sql_images_root;
  std::string sql_images_targets;
  std::string sql_images_timestamp;
  std::string sql_images_snapshot;

  sql_storage->loadLatestRoot(&sql_director_root, Uptane::RepositoryType::Director);
  sql_storage->loadNonRoot(&sql_director_targets, Uptane::RepositoryType::Director, Uptane::Role::Targets());
  sql_storage->loadLatestRoot(&sql_images_root, Uptane::RepositoryType::Images);
  sql_storage->loadNonRoot(&sql_images_targets, Uptane::RepositoryType::Images, Uptane::Role::Targets());
  sql_storage->loadNonRoot(&sql_images_timestamp, Uptane::RepositoryType::Images, Uptane::Role::Timestamp());
  sql_storage->loadNonRoot(&sql_images_snapshot, Uptane::RepositoryType::Images, Uptane::Role::Snapshot());

  EXPECT_EQ(sql_public_key, public_key);
  EXPECT_EQ(sql_private_key, private_key);
  EXPECT_EQ(sql_ca, ca);
  EXPECT_EQ(sql_cert, cert);
  EXPECT_EQ(sql_pkey, pkey);
  EXPECT_EQ(sql_device_id, device_id);
  EXPECT_EQ(sql_serials, serials);
  EXPECT_EQ(sql_ecu_registered, ecu_registered);
  EXPECT_EQ(sql_installed_versions, installed_versions);

  EXPECT_EQ(sql_director_root, director_root);
  EXPECT_EQ(sql_director_targets, director_targets);
  EXPECT_EQ(sql_images_root, images_root);
  EXPECT_EQ(sql_images_targets, images_targets);
  EXPECT_EQ(sql_images_timestamp, images_timestamp);
  EXPECT_EQ(sql_images_snapshot, images_snapshot);
}

TEST(Uptane, SaveVersion) {
  TemporaryDirectory temp_dir;
  Config config;
  config.storage.path = temp_dir.Path();
  config.storage.tls_cacert_path = BasedPath("ca.pem");
  config.storage.tls_clientcert_path = BasedPath("client.pem");
  config.storage.tls_pkey_path = BasedPath("pkey.pem");
  config.provision.device_id = "device_id";
  config.postUpdateValues();
  auto storage = INvStorage::newStorage(config.storage);
  auto http = std::make_shared<HttpFake>(temp_dir.Path());

  Json::Value target_json;
  target_json["hashes"]["sha256"] = "a0fb2e119cf812f1aa9e993d01f5f07cb41679096cb4492f1265bff5ac901d0d";
  target_json["length"] = 123;

  Uptane::Target t("target_name", target_json);
  storage->saveInstalledVersion(t);

  std::vector<Uptane::Target> installed_versions;
  storage->loadInstalledVersions(&installed_versions);

  auto f = std::find_if(installed_versions.begin(), installed_versions.end(),
                        [](const Uptane::Target& t_) { return t_.filename() == "target_name"; });
  EXPECT_NE(f, installed_versions.end());
  EXPECT_EQ(f->sha256Hash(), "a0fb2e119cf812f1aa9e993d01f5f07cb41679096cb4492f1265bff5ac901d0d");
  EXPECT_EQ(f->length(), 123);
}

TEST(Uptane, LoadVersion) {
  TemporaryDirectory temp_dir;
  Config config;
  config.storage.path = temp_dir.Path();
  config.storage.tls_cacert_path = BasedPath("ca.pem");
  config.storage.tls_clientcert_path = BasedPath("client.pem");
  config.storage.tls_pkey_path = BasedPath("pkey.pem");
  config.provision.device_id = "device_id";
  config.postUpdateValues();
  auto storage = INvStorage::newStorage(config.storage);
  auto http = std::make_shared<HttpFake>(temp_dir.Path());

  Json::Value target_json;
  target_json["hashes"]["sha256"] = "a0fb2e119cf812f1aa9e993d01f5f07cb41679096cb4492f1265bff5ac901d0d";
  target_json["length"] = 0;

  Uptane::Target t("target_name", target_json);
  storage->saveInstalledVersion(t);

  std::vector<Uptane::Target> versions;
  storage->loadInstalledVersions(&versions);
  EXPECT_EQ(t, versions[0]);
}

TEST(Uptane, krejectallTest) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());

  Config config;
  config.storage.path = temp_dir.Path();
  config.uptane.director_server = http->tls_server + "/director";
  config.uptane.repo_server = http->tls_server + "/repo";
  config.storage.type = StorageType::kSqlite;
  config.pacman.type = PackageManager::kNone;
  config.provision.device_id = "device_id";
  config.postUpdateValues();

  auto storage = INvStorage::newStorage(config.storage);
  auto sota_client = SotaUptaneClient::newTestClient(config, storage, http);
  Uptane::EcuSerial primary_ecu_serial("CA:FE:A6:D2:84:9D");
  Uptane::HardwareIdentifier primary_hw_id("primary_hw");
  Uptane::EcuSerial secondary_ecu_serial("secondary_ecu_serial");
  Uptane::HardwareIdentifier secondary_hw_id("secondary_hw");
  sota_client->hw_ids.insert(std::make_pair(primary_ecu_serial, primary_hw_id));
  sota_client->hw_ids.insert(std::make_pair(secondary_ecu_serial, secondary_hw_id));
  EXPECT_TRUE(sota_client->uptaneIteration());
}

TEST(Uptane, restoreVerify) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config config("tests/config/basic.toml");
  config.storage.path = temp_dir.Path();
  config.uptane.director_server = http->tls_server + "/unstable/director";
  config.uptane.repo_server = http->tls_server + "/unstable/repo";
  config.pacman.type = PackageManager::kNone;
  config.provision.primary_ecu_serial = "CA:FE:A6:D2:84:9D";
  config.provision.primary_ecu_hardware_id = "primary_hw";
  config.storage.uptane_metadata_path = BasedPath("metadata");
  config.storage.uptane_private_key_path = BasedPath("private.key");
  config.storage.uptane_public_key_path = BasedPath("public.key");
  addDefaultSecondary(config, temp_dir, "secondary_hw");
  config.postUpdateValues();

  auto storage = INvStorage::newStorage(config.storage);
  auto sota_client = SotaUptaneClient::newTestClient(config, storage, http);

  EXPECT_TRUE(sota_client->initialize());
  sota_client->AssembleManifest();
  // 1st attempt, don't get anything
  EXPECT_FALSE(sota_client->uptaneIteration());
  EXPECT_FALSE(storage->loadLatestRoot(nullptr, Uptane::RepositoryType::Director));

  // 2nd attempt, get director root.json
  EXPECT_FALSE(sota_client->uptaneIteration());
  EXPECT_TRUE(storage->loadLatestRoot(nullptr, Uptane::RepositoryType::Director));
  EXPECT_FALSE(storage->loadNonRoot(nullptr, Uptane::RepositoryType::Director, Uptane::Role::Targets()));

  // 3rd attempt, get director targets.json
  EXPECT_FALSE(sota_client->uptaneIteration());
  EXPECT_TRUE(storage->loadLatestRoot(nullptr, Uptane::RepositoryType::Director));
  EXPECT_TRUE(storage->loadNonRoot(nullptr, Uptane::RepositoryType::Director, Uptane::Role::Targets()));
  EXPECT_FALSE(storage->loadLatestRoot(nullptr, Uptane::RepositoryType::Images));

  // 4th attempt, get images root.json
  EXPECT_FALSE(sota_client->uptaneIteration());
  EXPECT_TRUE(storage->loadLatestRoot(nullptr, Uptane::RepositoryType::Images));
  EXPECT_FALSE(storage->loadNonRoot(nullptr, Uptane::RepositoryType::Images, Uptane::Role::Timestamp()));

  // 5th attempt, get images timestamp.json
  EXPECT_FALSE(sota_client->uptaneIteration());
  EXPECT_TRUE(storage->loadNonRoot(nullptr, Uptane::RepositoryType::Images, Uptane::Role::Timestamp()));
  EXPECT_FALSE(storage->loadNonRoot(nullptr, Uptane::RepositoryType::Images, Uptane::Role::Snapshot()));

  // 6th attempt, get images snapshot.json
  EXPECT_FALSE(sota_client->uptaneIteration());
  EXPECT_TRUE(storage->loadNonRoot(nullptr, Uptane::RepositoryType::Images, Uptane::Role::Snapshot()));
  EXPECT_FALSE(storage->loadNonRoot(nullptr, Uptane::RepositoryType::Images, Uptane::Role::Targets()));

  // 7th attempt, get images targets.json, successful iteration
  EXPECT_TRUE(sota_client->uptaneIteration());
  EXPECT_TRUE(storage->loadNonRoot(nullptr, Uptane::RepositoryType::Images, Uptane::Role::Targets()));
}

TEST(Uptane, offlineIteration) {
  TemporaryDirectory temp_dir;
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  Config config("tests/config/basic.toml");
  config.storage.path = temp_dir.Path();
  config.uptane.director_server = http->tls_server + "director";
  config.uptane.repo_server = http->tls_server + "repo";
  config.pacman.type = PackageManager::kNone;
  config.provision.primary_ecu_serial = "CA:FE:A6:D2:84:9D";
  config.provision.primary_ecu_hardware_id = "primary_hw";
  config.storage.uptane_metadata_path = BasedPath("metadata");
  config.storage.uptane_private_key_path = BasedPath("private.key");
  config.storage.uptane_public_key_path = BasedPath("public.key");
  addDefaultSecondary(config, temp_dir, "secondary_hw");
  config.postUpdateValues();

  auto storage = INvStorage::newStorage(config.storage);
  std::shared_ptr<event::Channel> events_channel{new event::Channel};
  auto sota_client = SotaUptaneClient::newTestClient(config, storage, http, events_channel);

  EXPECT_TRUE(sota_client->initialize());
  sota_client->AssembleManifest();

  std::vector<Uptane::Target> targets_online;
  EXPECT_TRUE(sota_client->uptaneIteration());
  EXPECT_TRUE(sota_client->getNewTargets(&targets_online));

  std::vector<Uptane::Target> targets_offline;
  EXPECT_TRUE(sota_client->uptaneOfflineIteration(&targets_offline, nullptr));
  EXPECT_EQ(targets_online, targets_offline);
}

#ifdef BUILD_P11
TEST(Uptane, Pkcs11Provision) {
  Config config;
  TemporaryDirectory temp_dir;
  boost::filesystem::copy_file("tests/test_data/implicit/ca.pem", temp_dir / "ca.pem");
  config.tls.cert_source = CryptoSource::kPkcs11;
  config.tls.pkey_source = CryptoSource::kPkcs11;
  config.p11.module = TEST_PKCS11_MODULE_PATH;
  config.p11.pass = "1234";
  config.p11.tls_clientcert_id = "01";
  config.p11.tls_pkey_id = "02";

  config.storage.path = temp_dir.Path();
  config.storage.tls_cacert_path = BasedPath("ca.pem");
  config.postUpdateValues();

  auto storage = INvStorage::newStorage(config.storage);
  auto http = std::make_shared<HttpFake>(temp_dir.Path());
  KeyManager keys(storage, config.keymanagerConfig());
  Initializer initializer(config.provision, storage, http, keys, {});

  EXPECT_TRUE(initializer.isSuccessful());
}
#endif

#ifndef __NO_MAIN__
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  logger_init();
  logger_set_threshold(boost::log::trivial::trace);

  if (argc != 2) {
    std::cerr << "Error: " << argv[0] << " requires the path to an OSTree sysroot as an input argument.\n";
    return EXIT_FAILURE;
  }
  sysroot = argv[1];
  return RUN_ALL_TESTS();
}
#endif
