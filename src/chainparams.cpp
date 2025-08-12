// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2025 The PureCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/params.h>
#include <arith_uint256.h>
#include <tinyformat.h>
#include <util/system.h>
#include <assert.h>
#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "PureCoin genesis block - 2025-08-12";
    const CScript genesisOutputScript = CScript() << OP_0 << ParseHex("3291c836314c9b704710c0e057931f1c3f9a242c");

    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) 
        << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "purecoin-main";

        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xb2;
        pchMessageStart[2] = 0xc3;
        pchMessageStart[3] = 0xd4;

        nDefaultPort = 28333;
        nPruneAfterHeight = 100000;

        consensus.nSubsidyHalvingInterval = 210000;
        consensus.nPowTargetSpacing = 45; // 45 seconds per block

        consensus.BIP16Exception = uint256S("0x0");
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.CSVHeight = 0;
        consensus.SegwitHeight = std::numeric_limits<int>::max(); // Disable Segwit

        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nRuleChangeActivationThreshold = 1815; // 90% of 2016
        consensus.nMinerConfirmationWindow = 2016;

        consensus.nMaxBlockWeight = 500000; 
        consensus.nMaxSerializedBlockSize = 500000;

        genesis = CreateGenesisBlock(1691800000, 214878, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x000007029b560c8b619ca32b1685d75ea8dfa884730180faa45bf7d2e4c0b9b9"));
        assert(genesis.hashMerkleRoot == uint256S("0xd131e7aaf25cc7c7bb5a2b18bc235a6a835f6d70e864ab6b7f28b934c1ec9d2c"));

        vSeeds.clear(); // Add seed nodes here if you have any

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,28); // Prefix 'P'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);

        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "pc";

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;
        m_is_mockable_chain = false;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const override {
        static Checkpoints::CCheckpointData data {
            {
                {0, consensus.hashGenesisBlock}
            }
        };
        return data;
    }
};

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, const ChainType chain)
{
    switch(chain) {
    case ChainType::MAIN:
        return std::make_unique<CMainParams>();
    case ChainType::TESTNET:
        throw std::runtime_error("PureCoin TESTNET not implemented");
    case ChainType::REGTEST:
        throw std::runtime_error("PureCoin REGTEST not implemented");
    default:
        throw std::runtime_error("Unknown chain");
    }
}

void SelectParams(const ChainType chain)
{
    SelectBaseParams(chain);
    globalChainParams = CreateChainParams(gArgs, chain);
}
