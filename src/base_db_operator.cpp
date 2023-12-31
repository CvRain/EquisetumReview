//
// Created by cvrain on 23-10-10.
//

#include "base_db_operator.h"

namespace DbOperator {
    BaseDbOperator::BaseDbOperator(const std::filesystem::path &db_path) {
        LOG_F(INFO, "BaseDbOperator::init %s", db_path.string().data());
        if(!std::filesystem::exists(db_path.parent_path())){
            std::filesystem::create_directories(db_path.parent_path());
            LOG_F(INFO,"Could not found default directory, create...");
        }
        leveldb::Options options;
        options.create_if_missing = true;
        const auto status = leveldb::DB::Open(options, db_path, &db);
        if (!status.ok()) {
            LOG_F(ERROR, "%s", status.ToString().data());
            assert(status.ok());
        }
        LOG_F(INFO, "Open level db: %s", db_path.string().data());
    }

    void BaseDbOperator::RemoveDb() {
        delete db;
        db = nullptr;
        LOG_F(INFO, "Delete database successfully");
    }

    BaseDbOperator::~BaseDbOperator() {
        if (db != nullptr) {
            RemoveDb();
        }
    }

    DbResult BaseDbOperator::Put(const std::string_view &key, const std::string_view &value) {
        const auto status = db->Put(leveldb::WriteOptions(), key.data(), value.data());
        CheckDbOperatorStatus(status);
        return std::make_pair(status, value);
    }

    DbResult BaseDbOperator::Get(const std::string_view &key) {
        std::string value;
        const auto status = db->Get(leveldb::ReadOptions(), key.data(), &value);
        CheckDbOperatorStatus(status);
        return std::make_pair(status, value);
    }

    DbResultList BaseDbOperator::GetAll() {
        DbKeyValue result{};
        auto iter = db->NewIterator(leveldb::ReadOptions());
        for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
            result.insert(std::make_pair(iter->key().data(), iter->value().data()));
        }
        const auto status = iter->status();
        delete iter;
        CheckDbOperatorStatus(status);
        return DbResultList{status, result};
    }

    DbResult BaseDbOperator::Delete(const std::string_view &key) {
        const auto status = db->Delete(leveldb::WriteOptions(), key.data());
        CheckDbOperatorStatus(status);
        return std::make_pair(status, key);
    }

    std::string BaseDbOperator::CheckDbOperatorStatus(const leveldb::Status &status) {
        if(!status.ok()){
            LOG_F(ERROR, "Error: %s", status.ToString().data());
        }
        return status.ToString();
    }

}