#pragma once

// This is not C

enum Opcodes
{
	// Assign
	kSetVxToVy = 0x8000,

	// Condition
	kSkipNextIfVxEquals = 0x3000,
	kSkipNextIfVxDifferent = 0x4000,
	kSkipNextIfVxEqualsVy = 0x5000,
	kSkipNextIfVxDifferentVy = 0x9000,

	// Const
	kSetVxToNN = 0x6000,
	kAddNNToVx = 0x7000,

	// Display
	kClearScreen = 0x00e0,
	kDrawSprite = 0xd000,

	// Flow
	kReturn = 0x00ee,
	kGoto = 0x1000,
	kCallSubroutine = 0x2000,
	kJumpNNNPlusV0 = 0xb000,

	// Key operations
	kSkipNextIfKeyPressed = 0xe09e,
	kSkipNextIfKeyNotPressed = 0xe0a1,
	kReadKey = 0xf00a,

	// Math
	kVxOrVy = 0x8001,
	kVxAndVy = 0x8002,
	kVxXorVy = 0x8003,
	kVxPlusVy = 0x8004,
	kVxMinusVy = 0x8005,
	kVxRightShift = 0x8006,
	kVyMinuxVx = 0x8007,
	kVxLeftShift = 0x800e,
	kRandAndNN = 0xc000,

	// Memory
	kSetI = 0xa000,
	kAddVxToI = 0xf01e,
	kSetIToChar = 0xf029,
	kStoreRegistersAtI = 0xf055,
	kFillRegistersFromI = 0xf065,

	// Timer
	kSetToDelayTimer = 0xf007,
	kSetDelayTimer = 0xf015,
	kSetSoundTimer = 0xf018,

	// Others
	kSetIToBCD = 0xf033,
};
