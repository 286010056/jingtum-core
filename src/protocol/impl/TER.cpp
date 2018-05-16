//------------------------------------------------------------------------------
/*
    This file is part of skywelld: https://github.com/skywell/skywelld
    Copyright (c) 2012, 2013 Skywell Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#include <BeastConfig.h>
#include <protocol/TER.h>

namespace skywell {

bool transResultInfo (TER code, std::string& token, std::string& text)
{
    struct TxResultInfo
    {
        TER code;
        char const* token;
        char const* text;
    };

    // FIXME: replace this with a function-static std::map and the lookup
    // code with std::map::find when the problem with magic statics on
    // Visual Studio is fixed.
    static
    TxResultInfo const results[] =
    {
        { tecCLAIM,                 "tecCLAIM",                 "Fee claimed. Sequence used. No action."                        },
        { tecDIR_FULL,              "tecDIR_FULL",              "Can not add entry to full directory."                          },
        { tecFAILED_PROCESSING,     "tecFAILED_PROCESSING",     "Failed to correctly process transaction."                      },
        { tecINSUF_RESERVE_LINE,    "tecINSUF_RESERVE_LINE",    "Insufficient reserve to add trust line."                       },
        { tecINSUF_RESERVE_OFFER,   "tecINSUF_RESERVE_OFFER",   "Insufficient reserve to create offer."                         },
        { tecNO_DST,                "tecNO_DST",                "Destination does not exist. Send SWT to create it."            },
        { tecNO_DST_INSUF_SWT,      "tecNO_DST_INSUF_SWT",      "Destination does not exist. Too little SWT sent to create it." },
        { tecNO_LINE_INSUF_RESERVE, "tecNO_LINE_INSUF_RESERVE", "No such line. Too little reserve to create it."                },
        { tecNO_LINE_REDUNDANT,     "tecNO_LINE_REDUNDANT",     "Can't set non-existent line to default."                       },
        { tecPATH_DRY,              "tecPATH_DRY",              "Path could not send partial amount."                           },
        { tecPATH_PARTIAL,          "tecPATH_PARTIAL",          "Path could not send full amount."                              },
        { tecMASTER_DISABLED,       "tecMASTER_DISABLED",       "Master key is disabled."                                       },
        { tecNO_REGULAR_KEY,        "tecNO_REGULAR_KEY",        "Regular key is not set."                                       },

        { tecUNFUNDED,              "tecUNFUNDED",              "One of _ADD, _OFFER, or _SEND. Deprecated."                    },
        { tecUNFUNDED_ADD,          "tecUNFUNDED_ADD",          "Insufficient SWT balance for WalletAdd."                       },
        { tecUNFUNDED_OFFER,        "tecUNFUNDED_OFFER",        "Insufficient balance to fund created offer."                   },
        { tecUNFUNDED_PAYMENT,      "tecUNFUNDED_PAYMENT",      "Insufficient SWT balance to send."                             },
        { tecOWNERS,                "tecOWNERS",                "Non-zero owner count."                                         },
        { tecNO_ISSUER,             "tecNO_ISSUER",             "Issuer account does not exist."                                },
        { tecNO_AUTH,               "tecNO_AUTH",               "Not authorized to hold asset."                                 },
        { tecNO_LINE,               "tecNO_LINE",               "No such line."                                                 },
        { tecINSUFF_FEE,            "tecINSUFF_FEE",            "Insufficient balance to pay fee."                              },
        { tecFROZEN,                "tecFROZEN",                "Asset is frozen."                                              },
        { tecNO_TARGET,             "tecNO_TARGET",             "Target account does not exist."                                },
        { tecNO_PERMISSION,         "tecNO_PERMISSION",         "No permission to perform requested operation."                 },
        { tecNO_ENTRY,              "tecNO_ENTRY",              "No matching entry found."                                      },
        { tecINSUFFICIENT_RESERVE,  "tecINSUFFICIENT_RESERVE",  "Insufficient reserve to complete requested operation."         },
        { tecNEED_MASTER_KEY,       "tecNEED_MASTER_KEY",       "The operation requires the use of the Master Key."             },
        { tecDST_TAG_NEEDED,        "tecDST_TAG_NEEDED",        "A destination tag is required."                                },
        { tecINTERNAL,              "tecINTERNAL",              "An internal error has occurred during processing."             },

        { tefALREADY,               "tefALREADY",               "The exact transaction was already in this ledger."             },
        { tefBAD_ADD_AUTH,          "tefBAD_ADD_AUTH",          "Not authorized to add account."                                },
        { tefBAD_AUTH,              "tefBAD_AUTH",              "Transaction's public key is not authorized."                   },
        { tefBAD_LEDGER,            "tefBAD_LEDGER",            "Ledger in unexpected state."                                   },
        { tefCREATED,               "tefCREATED",               "Can't add an already created account."                         },
        { tefEXCEPTION,             "tefEXCEPTION",             "Unexpected program state."                                     },
        { tefFAILURE,               "tefFAILURE",               "Failed to apply."                                              },
        { tefINTERNAL,              "tefINTERNAL",              "Internal error."                                               },
        { tefMASTER_DISABLED,       "tefMASTER_DISABLED",       "Master key is disabled."                                       },
        { tefMAX_LEDGER,            "tefMAX_LEDGER",            "Ledger sequence too high."                                     },
        { tefNO_AUTH_REQUIRED,      "tefNO_AUTH_REQUIRED",      "Auth is not required."                                         },
        { tefPAST_SEQ,              "tefPAST_SEQ",              "This sequence number has already past."                        },
        { tefWRONG_PRIOR,           "tefWRONG_PRIOR",           "This previous transaction does not match."                     },

        { telLOCAL_ERROR,           "telLOCAL_ERROR",           "Local failure."                                                },
        { telBAD_DOMAIN,            "telBAD_DOMAIN",            "Domain too long."                                              },
        { telBAD_PATH_COUNT,        "telBAD_PATH_COUNT",        "Malformed: Too many paths."                                    },
        { telBAD_PUBLIC_KEY,        "telBAD_PUBLIC_KEY",        "Public key too long."                                          },
        { telFAILED_PROCESSING,     "telFAILED_PROCESSING",     "Failed to correctly process transaction."                      },
        { telINSUF_FEE_P,           "telINSUF_FEE_P",           "Fee insufficient."                                             },
        { telNO_DST_PARTIAL,        "telNO_DST_PARTIAL",        "Partial payment to create account not allowed."                },
        { telBLKLIST,          "telBLKLIST",          "Tx disable for blacklist."                                        },
        { telINSUF_FUND,           "telINSUF_FUND",           "Fund insufficient."                                             },

        { temMALFORMED,             "temMALFORMED",             "Malformed transaction."                                        },
        { temBAD_AMOUNT,            "temBAD_AMOUNT",            "Can only send positive amounts."                               },
        { temBAD_AUTH_MASTER,       "temBAD_AUTH_MASTER",       "Auth for unclaimed account needs correct master key."          },
        { temBAD_CURRENCY,          "temBAD_CURRENCY",          "Malformed: Bad currency."                                      },
        { temBAD_EXPIRATION,        "temBAD_EXPIRATION",        "Malformed: Bad expiration."                                    },
        { temBAD_FEE,               "temBAD_FEE",               "Invalid fee, negative or not SWT."                             },
        { temBAD_ISSUER,            "temBAD_ISSUER",            "Malformed: Bad issuer."                                        },
        { temBAD_LIMIT,             "temBAD_LIMIT",             "Limits must be non-negative."                                  },
		{ temBAD_QUORUM,			"temBAD_QUORUM",			"Quorums must be non-negative."									},
		{ temBAD_WEIGHT,			"temBAD_WEIGHT",			"Weights must be non-negative."									},
		{ temBAD_OFFER,             "temBAD_OFFER",             "Malformed: Bad offer."                                         },
        { temBAD_PATH,              "temBAD_PATH",              "Malformed: Bad path."                                          },
        { temBAD_PATH_LOOP,         "temBAD_PATH_LOOP",         "Malformed: Loop in path."                                      },
        { temBAD_SEND_SWT_LIMIT,    "temBAD_SEND_SWT_LIMIT",    "Malformed: Limit quality is not allowed for SWT to SWT."       },
        { temBAD_SEND_SWT_MAX,      "temBAD_SEND_SWT_MAX",      "Malformed: Send max is not allowed for SWT to SWT."            },
        { temBAD_SEND_SWT_NO_DIRECT,"temBAD_SEND_SWT_NO_DIRECT","Malformed: No Skywell direct is not allowed for SWT to SWT."    },
        { temBAD_SEND_SWT_PARTIAL,  "temBAD_SEND_SWT_PARTIAL",  "Malformed: Partial payment is not allowed for SWT to SWT."     },
        { temBAD_SEND_SWT_PATHS,    "temBAD_SEND_SWT_PATHS",    "Malformed: Paths are not allowed for SWT to SWT."              },
        { temBAD_SEQUENCE,          "temBAD_SEQUENCE",          "Malformed: Sequence is not in the past."                       },
        { temBAD_SIGNATURE,         "temBAD_SIGNATURE",         "Malformed: Bad signature."                                     },
        { temBAD_SRC_ACCOUNT,       "temBAD_SRC_ACCOUNT",       "Malformed: Bad source account."                                },
        { temBAD_TRANSFER_RATE,     "temBAD_TRANSFER_RATE",     "Malformed: Transfer rate must be >= 1.0"                       },
        { temDST_IS_SRC,            "temDST_IS_SRC",            "Destination may not be source."                                },
        { temDST_NEEDED,            "temDST_NEEDED",            "Destination not specified."                                    },
        { temINVALID,               "temINVALID",               "The transaction is ill-formed."                                },
        { temINVALID_FLAG,          "temINVALID_FLAG",          "The transaction has an invalid flag."                          },
        { temREDUNDANT,             "temREDUNDANT",             "Sends same currency to self."                                  },
		{ temREDUNDANTSIGN,			"temREDUNDANTSIGN",			"Add self as additional sign."									},
        { temSKYWELL_EMPTY,          "temSKYWELL_EMPTY",          "PathSet with no paths."                                        },
        { temUNCERTAIN,             "temUNCERTAIN",             "In process of determining result. Never returned."             },
        { temUNKNOWN,               "temUNKNOWN",               "The transaction requires logic that is not implemented yet."   },
        { temDISABLED,              "temDISABLED",              "The transaction requires logic that is currently disabled."    },

        { terRETRY,                 "terRETRY",                 "Retry transaction."                                            },
        { terFUNDS_SPENT,           "terFUNDS_SPENT",           "Can't set password, password set funds already spent."         },
        { terINSUF_FEE_B,           "terINSUF_FEE_B",           "Account balance can't pay fee."                                },
        { terLAST,                  "terLAST",                  "Process last."                                                 },
        { terNO_SKYWELL,             "terNO_SKYWELL",             "Path does not permit rippling."                                },
        { terNO_ACCOUNT,            "terNO_ACCOUNT",            "The source account does not exist."                            },
        { terNO_AUTH,               "terNO_AUTH",               "Not authorized to hold IOUs."                                  },
        { terNO_LINE,               "terNO_LINE",               "No such line."                                                 },
        { terPRE_SEQ,               "terPRE_SEQ",               "Missing/inapplicable prior transaction."                       },
        { terOWNERS,                "terOWNERS",                "Non-zero owner count."                                         },

        { tesSUCCESS,               "tesSUCCESS",               "The transaction was applied. Only final in a validated ledger." },
    };

    for (auto const& result : results)
    {
        if (result.code == code)
        {
            token = result.token;
            text = result.text;
            return true;
        }
    }

    return false;
}

std::string transToken (TER code)
{
    std::string token;
    std::string text;

    return transResultInfo (code, token, text) ? token : "-";
}

std::string transHuman (TER code)
{
    std::string token;
    std::string text;

    return transResultInfo (code, token, text) ? text : "-";
}

} // skywell