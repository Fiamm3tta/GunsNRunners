#pragma once

UENUM()
enum class EGNRConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EGNRValidType : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EGNRSuccessType : uint8
{
	Successful,
	Failed
};