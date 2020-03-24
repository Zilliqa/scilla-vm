; let one = Int256 -1 in
; let two = Int256 2 in
; builtin add one two
; ModuleID = 'scilla_expr'
source_filename = "scilla_expr"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"$TyDescrTy_PrimTyp_1" = type { i32, i32 }
%_TyDescrTy_Typ = type { i32, i8* }
%Int256 = type { i256 }

@_execptr = global i8* null
@"$TyDescr_Int32_Prim_2" = global %"$TyDescrTy_PrimTyp_1" zeroinitializer
@"$TyDescr_Int32_3" = global %_TyDescrTy_Typ { i32 0, i8* bitcast (%"$TyDescrTy_PrimTyp_1"* @"$TyDescr_Int32_Prim_2" to i8*) }
@"$TyDescr_Uint32_Prim_4" = global %"$TyDescrTy_PrimTyp_1" { i32 1, i32 0 }
@"$TyDescr_Uint32_5" = global %_TyDescrTy_Typ { i32 0, i8* bitcast (%"$TyDescrTy_PrimTyp_1"* @"$TyDescr_Uint32_Prim_4" to i8*) }
@"$TyDescr_Int64_Prim_6" = global %"$TyDescrTy_PrimTyp_1" { i32 0, i32 1 }
@"$TyDescr_Int64_7" = global %_TyDescrTy_Typ { i32 0, i8* bitcast (%"$TyDescrTy_PrimTyp_1"* @"$TyDescr_Int64_Prim_6" to i8*) }
@"$TyDescr_Uint64_Prim_8" = global %"$TyDescrTy_PrimTyp_1" { i32 1, i32 1 }
@"$TyDescr_Uint64_9" = global %_TyDescrTy_Typ { i32 0, i8* bitcast (%"$TyDescrTy_PrimTyp_1"* @"$TyDescr_Uint64_Prim_8" to i8*) }
@"$TyDescr_Int128_Prim_10" = global %"$TyDescrTy_PrimTyp_1" { i32 0, i32 2 }
@"$TyDescr_Int128_11" = global %_TyDescrTy_Typ { i32 0, i8* bitcast (%"$TyDescrTy_PrimTyp_1"* @"$TyDescr_Int128_Prim_10" to i8*) }
@"$TyDescr_Uint128_Prim_12" = global %"$TyDescrTy_PrimTyp_1" { i32 1, i32 2 }
@"$TyDescr_Uint128_13" = global %_TyDescrTy_Typ { i32 0, i8* bitcast (%"$TyDescrTy_PrimTyp_1"* @"$TyDescr_Uint128_Prim_12" to i8*) }
@"$TyDescr_Int256_Prim_14" = global %"$TyDescrTy_PrimTyp_1" { i32 0, i32 3 }
@"$TyDescr_Int256_15" = global %_TyDescrTy_Typ { i32 0, i8* bitcast (%"$TyDescrTy_PrimTyp_1"* @"$TyDescr_Int256_Prim_14" to i8*) }
@"$TyDescr_Uint256_Prim_16" = global %"$TyDescrTy_PrimTyp_1" { i32 1, i32 3 }
@"$TyDescr_Uint256_17" = global %_TyDescrTy_Typ { i32 0, i8* bitcast (%"$TyDescrTy_PrimTyp_1"* @"$TyDescr_Uint256_Prim_16" to i8*) }
@"$TyDescr_String_Prim_18" = global %"$TyDescrTy_PrimTyp_1" { i32 2, i32 0 }
@"$TyDescr_String_19" = global %_TyDescrTy_Typ { i32 0, i8* bitcast (%"$TyDescrTy_PrimTyp_1"* @"$TyDescr_String_Prim_18" to i8*) }
@"$TyDescr_Bystr_Prim_20" = global %"$TyDescrTy_PrimTyp_1" { i32 7, i32 0 }
@"$TyDescr_Bystr_21" = global %_TyDescrTy_Typ { i32 0, i8* bitcast (%"$TyDescrTy_PrimTyp_1"* @"$TyDescr_Bystr_Prim_20" to i8*) }

define internal void @"$scilla_expr_26"(i8*, %Int256*) {
entry:
  %"$expr_0" = alloca %Int256
  %one = alloca %Int256
  store %Int256 { i256 -1 }, %Int256* %one
  %two = alloca %Int256
  store %Int256 { i256 2 }, %Int256* %two
  %"$add_one_27" = alloca %Int256
  %"$one_28" = load %Int256, %Int256* %one
  store %Int256 %"$one_28", %Int256* %"$add_one_27"
  %"$add_two_29" = alloca %Int256
  %"$two_30" = load %Int256, %Int256* %two
  store %Int256 %"$two_30", %Int256* %"$add_two_29"
  %"$add_retalloca_31" = alloca %Int256
  call void @_add_Int256(%Int256* %"$add_retalloca_31", %Int256* %"$add_one_27", %Int256* %"$add_two_29")
  %"$add_ret_32" = load %Int256, %Int256* %"$add_retalloca_31"
  store %Int256 %"$add_ret_32", %Int256* %"$expr_0"
  %"$$expr_0_33" = load %Int256, %Int256* %"$expr_0"
  store %Int256 %"$$expr_0_33", %Int256* %1
  ret void
}

declare void @_add_Int256(%Int256*, %Int256*, %Int256*)

declare void @_print_scilla_val(%_TyDescrTy_Typ*, i8*)

define void @scilla_main() {
entry:
  %"$mainval_34" = alloca %Int256
  %"$memvoidcast_35" = bitcast %Int256* %"$mainval_34" to i8*
  call void @"$scilla_expr_26"(i8* null, %Int256* %"$mainval_34")
  call void @_print_scilla_val(%_TyDescrTy_Typ* @"$TyDescr_Int256_15", i8* %"$memvoidcast_35")
  ret void
}
