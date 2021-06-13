/*
 * Copyright (C) 2020 Zilliqa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

/* This file declares functions that are callable from an executing
 * Scilla contract. It is not intended to be called from other C++
 * code, and is therefore not a public header. */

#include "ScillaRTL/ScillaExec.h"

#include "SafeInt.h"
#include "ScillaTypes.h"
#include "ScillaValue.h"

namespace ScillaRTL {

// Values that begin with a transition and change as it executes.
class TransitionState {
  SafeUint128 Balance;
  SafeUint128 InAmount;
  uint64_t *GasRemPtr;
  bool Accepted;
  // Contains the output messages of executing a transition.
  Json::Value OutJ;

  void processMessage(std::string OutType, Json::Value &M);

public:
  TransitionState(std::string Balance_P, std::string InAmount_P,
                  uint64_t *GasRemPtr_P)
      : Balance(Balance_P), InAmount(InAmount_P), GasRemPtr(GasRemPtr_P),
        Accepted(false), OutJ(Json::objectValue){};

  void processSend(Json::Value &M);
  void processEvent(Json::Value &M);
  void processAccept();

  // Returns the output of the transition execution. Destroys *this*.
  Json::Value finalize();
};

} // end of namespace ScillaRTL

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"

extern "C" {

// Print Scilla value @V whose type is described by @T to ScillaStdout .
void _print_scilla_val(ScillaRTL::ScillaExecImpl *SJ,
                       const ScillaRTL::ScillaTypes::Typ *T, void *V);

// Allocate memory for JIT code owned by @SJ
void *_salloc(ScillaRTL::ScillaExecImpl *SJ, size_t size);

// Handler for out-of-gas during execution
void _out_of_gas();

// Fetch field @Name whose type is @T. For map accesses, FetchVal can be false,
// to indicate that the return value is a Scilla `Bool`, indicating found or
// not.
void *_fetch_field(ScillaRTL::ScillaExecImpl *SJ, const char *Name,
                   const ScillaRTL::ScillaTypes::Typ *T, int32_t NumIdx,
                   const uint8_t *Indices, int32_t FetchVal);

// Same as _fetch_field, but with an address parameter.
void *
_fetch_remote_field(ScillaRTL::ScillaExecImpl *SJ,
                    const uint8_t Addr[ScillaRTL::ScillaTypes::AddrByStr_Len],
                    const char *Name, const ScillaRTL::ScillaTypes::Typ *T,
                    int32_t NumIdx, const uint8_t *Indices, int32_t FetchVal);

// Update field @Name whose type is @T with value @Val. If Val is nullptr,
// then this is a map update and the keys must be deleted.
void _update_field(ScillaRTL::ScillaExecImpl *SJ, const char *Name,
                   const ScillaRTL::ScillaTypes::Typ *, int32_t NumIdx,
                   const uint8_t *Indices, void *Val);

// Integer addition builtins
ScillaRTL::ScillaTypes::Int32 _add_Int32(ScillaRTL::ScillaTypes::Int32,
                                         ScillaRTL::ScillaTypes::Int32);
ScillaRTL::ScillaTypes::Int64 _add_Int64(ScillaRTL::ScillaTypes::Int64,
                                         ScillaRTL::ScillaTypes::Int64);
ScillaRTL::ScillaTypes::Int128 _add_Int128(ScillaRTL::ScillaTypes::Int128,
                                           ScillaRTL::ScillaTypes::Int128);
ScillaRTL::ScillaTypes::Int256 *
_add_Int256(ScillaRTL::ScillaExecImpl *SJ, ScillaRTL::ScillaTypes::Int256 *Lhs,
            ScillaRTL::ScillaTypes::Int256 *Rhs);

ScillaRTL::ScillaTypes::Uint32 _add_Uint32(ScillaRTL::ScillaTypes::Uint32,
                                           ScillaRTL::ScillaTypes::Uint32);
ScillaRTL::ScillaTypes::Uint64 _add_Uint64(ScillaRTL::ScillaTypes::Uint64,
                                           ScillaRTL::ScillaTypes::Uint64);
ScillaRTL::ScillaTypes::Uint128 _add_Uint128(ScillaRTL::ScillaTypes::Uint128,
                                             ScillaRTL::ScillaTypes::Uint128);
ScillaRTL::ScillaTypes::Uint256 *
_add_Uint256(ScillaRTL::ScillaExecImpl *SJ,
             ScillaRTL::ScillaTypes::Uint256 *Lhs,
             ScillaRTL::ScillaTypes::Uint256 *Rhs);

uint8_t *_eq_Int32(ScillaRTL::ScillaExecImpl *SJ,
                   ScillaRTL::ScillaTypes::Int32 Lhs,
                   ScillaRTL::ScillaTypes::Int32 Rhs);

uint8_t *_eq_Int64(ScillaRTL::ScillaExecImpl *SJ,
                   ScillaRTL::ScillaTypes::Int64 Lhs,
                   ScillaRTL::ScillaTypes::Int64 Rhs);

uint8_t *_eq_Int128(ScillaRTL::ScillaExecImpl *SJ,
                    ScillaRTL::ScillaTypes::Int128 Lhs,
                    ScillaRTL::ScillaTypes::Int128 Rhs);

uint8_t *_eq_Int256(ScillaRTL::ScillaExecImpl *SJ,
                    ScillaRTL::ScillaTypes::Int256 *Lhs,
                    ScillaRTL::ScillaTypes::Int256 *Rhs);

uint8_t *_eq_Uint32(ScillaRTL::ScillaExecImpl *SJ,
                    ScillaRTL::ScillaTypes::Uint32 Lhs,
                    ScillaRTL::ScillaTypes::Uint32 Rhs);

uint8_t *_eq_Uint64(ScillaRTL::ScillaExecImpl *SJ,
                    ScillaRTL::ScillaTypes::Uint64 Lhs,
                    ScillaRTL::ScillaTypes::Uint64 Rhs);

uint8_t *_eq_Uint128(ScillaRTL::ScillaExecImpl *SJ,
                     ScillaRTL::ScillaTypes::Uint128 Lhs,
                     ScillaRTL::ScillaTypes::Uint128 Rhs);

uint8_t *_eq_Uint256(ScillaRTL::ScillaExecImpl *SJ,
                     ScillaRTL::ScillaTypes::Uint256 *Lhs,
                     ScillaRTL::ScillaTypes::Uint256 *Rhs);

void *_to_nat(ScillaRTL::ScillaExecImpl *SJ, ScillaRTL::ScillaTypes::Uint32 UI);

void _send(ScillaRTL::ScillaExecImpl *SJ, const ScillaRTL::ScillaTypes::Typ *T,
           const void *V);

void _event(ScillaRTL::ScillaExecImpl *SJ, const ScillaRTL::ScillaTypes::Typ *T,
            const void *V);

void _throw(ScillaRTL::ScillaExecImpl *SJ, const ScillaRTL::ScillaTypes::Typ *T,
            const void *V);

uint8_t *_eq_String(ScillaRTL::ScillaExecImpl *SJ,
                    ScillaRTL::ScillaTypes::String Lhs,
                    ScillaRTL::ScillaTypes::String Rhs);

uint8_t *_eq_ByStr(ScillaRTL::ScillaExecImpl *SJ,
                   ScillaRTL::ScillaTypes::String Lhs,
                   ScillaRTL::ScillaTypes::String Rhs);

uint8_t *_eq_ByStrX(ScillaRTL::ScillaExecImpl *SJ, int X, uint8_t *Lhs,
                    uint8_t *Rhs);

ScillaRTL::ScillaTypes::String _to_bystr(ScillaRTL::ScillaExecImpl *SJ, int X,
                                         uint8_t *Buf);

ScillaRTL::ScillaTypes::String _to_string(ScillaRTL::ScillaExecImpl *SJ,
                                          const ScillaRTL::ScillaTypes::Typ *T,
                                          const void *V);

ScillaRTL::ScillaTypes::String _to_ascii(ScillaRTL::ScillaExecImpl *SJ,
                                         const uint8_t *S, int L);

void *_bystr_to_bystrx(ScillaRTL::ScillaExecImpl *SJ, int X,
                       ScillaRTL::ScillaTypes::String Str);

uint8_t *_bech32_to_bystr20(ScillaRTL::ScillaExecImpl *SJ,
                            ScillaRTL::ScillaTypes::String Prefix,
                            ScillaRTL::ScillaTypes::String Addr);
void *_bystr20_to_bech32(ScillaRTL::ScillaExecImpl *SJ,
                         ScillaRTL::ScillaTypes::String Prefix,
                         uint8_t *Addr20);

void *_uint32_to_bystrx(ScillaRTL::ScillaExecImpl *SJ,
                        ScillaRTL::ScillaTypes::Uint32 I);

void *_uint64_to_bystrx(ScillaRTL::ScillaExecImpl *SJ,
                        ScillaRTL::ScillaTypes::Uint64 I);

void *_uint128_to_bystrx(ScillaRTL::ScillaExecImpl *SJ,
                         ScillaRTL::ScillaTypes::Uint128 I);

void *_uint256_to_bystrx(ScillaRTL::ScillaExecImpl *SJ,
                         ScillaRTL::ScillaTypes::Uint256 *I);

ScillaRTL::ScillaTypes::Uint32 _bystrx_to_uint32(ScillaRTL::ScillaExecImpl *,
                                                 int X, void *BS);
ScillaRTL::ScillaTypes::Uint64 _bystrx_to_uint64(ScillaRTL::ScillaExecImpl *,
                                                 int X, void *BS);
ScillaRTL::ScillaTypes::Uint128 _bystrx_to_uint128(ScillaRTL::ScillaExecImpl *,
                                                   int X, void *BS);
ScillaRTL::ScillaTypes::Uint256 *
_bystrx_to_uint256(ScillaRTL::ScillaExecImpl *SJ, int X, void *BS);

void *_sha256hash(ScillaRTL::ScillaExecImpl *SJ,
                  const ScillaRTL::ScillaTypes::Typ *T, void *V);

void *_keccak256hash(ScillaRTL::ScillaExecImpl *SJ,
                     const ScillaRTL::ScillaTypes::Typ *T, void *V);

void *_ripemd160hash(ScillaRTL::ScillaExecImpl *SJ,
                     const ScillaRTL::ScillaTypes::Typ *T, void *V);

uint8_t *_schnorr_verify(ScillaRTL::ScillaExecImpl *SJ, uint8_t *PubK,
                         ScillaRTL::ScillaTypes::String Msg, uint8_t *Sign);

uint8_t *_schnorr_get_address(ScillaRTL::ScillaExecImpl *SJ, uint8_t *PubK);

uint8_t *_ecdsa_verify(ScillaRTL::ScillaExecImpl *SJ, uint8_t *PubK,
                       ScillaRTL::ScillaTypes::String Msg, uint8_t *Sign);

uint8_t *_ecdsa_recover_pk(ScillaRTL::ScillaExecImpl *SJ,
                           ScillaRTL::ScillaTypes::String Msg, uint8_t *Sign,
                           ScillaRTL::ScillaTypes::Uint32 RecID);

ScillaRTL::ScillaTypes::String
_concat_String(ScillaRTL::ScillaExecImpl *SJ,
               ScillaRTL::ScillaTypes::String Lhs,
               ScillaRTL::ScillaTypes::String Rhs);

ScillaRTL::ScillaTypes::String
_concat_ByStr(ScillaRTL::ScillaExecImpl *SJ, ScillaRTL::ScillaTypes::String Lhs,
              ScillaRTL::ScillaTypes::String Rhs);

void *_concat_ByStrX(ScillaRTL::ScillaExecImpl *SJ, int X1, uint8_t *Lhs,
                     int X2, uint8_t *Rhs);

ScillaRTL::ScillaTypes::String _substr_String(
    ScillaRTL::ScillaExecImpl *SJ, ScillaRTL::ScillaTypes::String Str,
    ScillaRTL::ScillaTypes::Uint32 Pos, ScillaRTL::ScillaTypes::Uint32 Len);

ScillaRTL::ScillaTypes::String
_substr_ByStr(ScillaRTL::ScillaExecImpl *SJ, ScillaRTL::ScillaTypes::String Str,
              ScillaRTL::ScillaTypes::Uint32 Pos,
              ScillaRTL::ScillaTypes::Uint32 Len);

ScillaRTL::ScillaTypes::Uint32
_strlen_String(ScillaRTL::ScillaTypes::String Str);
ScillaRTL::ScillaTypes::Uint32
_strlen_ByStr(ScillaRTL::ScillaTypes::String Str);

void _accept(ScillaRTL::ScillaExecImpl *SJ);

ScillaRTL::ScillaParams::MapValueT *
_new_empty_map(ScillaRTL::ScillaExecImpl *SJ);
ScillaRTL::ScillaParams::MapValueT *_put(ScillaRTL::ScillaExecImpl *SJ,
                                         const ScillaRTL::ScillaTypes::Typ *T,
                                         ScillaRTL::ScillaParams::MapValueT *M,
                                         void *K, void *V);
void *_get(ScillaRTL::ScillaExecImpl *SJ, const ScillaRTL::ScillaTypes::Typ *T,
           const ScillaRTL::ScillaParams::MapValueT *M, const void *K);
void *_contains(ScillaRTL::ScillaExecImpl *SJ,
                const ScillaRTL::ScillaTypes::Typ *T,
                const ScillaRTL::ScillaParams::MapValueT *M, const void *K);

void *_remove(ScillaRTL::ScillaExecImpl *SJ,
              const ScillaRTL::ScillaTypes::Typ *T,
              const ScillaRTL::ScillaParams::MapValueT *M, const void *K);

// Scilla builtin _size : The size of a map.
ScillaRTL::ScillaTypes::Uint32
_size(const ScillaRTL::ScillaParams::MapValueT *M);

uint64_t _literal_cost(const ScillaRTL::ScillaTypes::Typ *T, const void *V);
uint64_t _mapsortcost(const ScillaRTL::ScillaParams::MapValueT *M);

} // extern "C"
#pragma clang diagnostic pop