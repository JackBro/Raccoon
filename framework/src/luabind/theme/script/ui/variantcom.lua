------------------------------------------------------------------------------- UI/1.1 Direct UI Of Raccoon-- UI Script FrameWork toolkit.-- Author: Jeffery Zhou-- RCS ID: $Id: variantcom.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $-----------------------------------------------------------------------------function VariantChar(v)	return Variant.CreateVariant(Variant.EChar, v)endfunction VariantUChar(v)	return Variant.CreateVariant(Variant.EUChar, v)endfunction VariantBool(v)	return Variant.CreateVariant(Variant.EBool, v)endfunction VariantInt(v)	return Variant.CreateVariant(Variant.EInt, v)endfunction VariantUInt(v)	return Variant.CreateVariant(Variant.EUInt, v)endfunction VariantDouble(v)	return Variant.CreateVariant(Variant.EDouble, v)endfunction VariantFloat(v)	return Variant.CreateVariant(Variant.EFloat, v)endfunction VariantInt64(v)	return Variant.CreateVariant(Variant.EInt64, v)endfunction VariantUInt64(v)	return Variant.CreateVariant(Variant.EUInt64, v)endfunction VariantPtr(v)	return Variant.CreateVariant(Variant.EPtr, v)endfunction VariantRectF(v)	return Variant.CreateVariant(Variant.ERectF, v)endfunction VariantRect(v)	return Variant.CreateVariant(Variant.ERect, v)endfunction VariantPointF(v)	return Variant.CreateVariant(Variant.EPointF, v)endfunction VariantPoint(v)	return Variant.CreateVariant(Variant.EPoint, v)endfunction VariantSizeF(v)	return Variant.CreateVariant(Variant.ESizeF, v)endfunction VariantSize(v)	return Variant.CreateVariant(Variant.ESize, v)endfunction VariantString(v)	return Variant.CreateVariant(Variant.EString, v)endfunction VariantReal(v)	return Variant.CreateVariant(Variant.EReal, v)endfunction VariantInvalid(v)	return Variant.CreateVariant(Variant.EInvalid, v)end