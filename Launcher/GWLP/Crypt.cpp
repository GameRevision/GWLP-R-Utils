/**
* ASM Rulez :P
**/

#include "Crypt.h"

BYTE* InjectReturn;
BYTE* PassJBackOff;

unsigned char* PasswordStore;

void __declspec(naked) ASMInjectPassword()
{
	_asm
	{
		PUSH EAX
		PUSH EBX
		PUSH ECX
		PUSH EDX
		PUSH EDI

		MOV EDI,ESI
		MOV ESI,EAX
		ADD ESI,0x01

		MOV EBX,EAX
		CMP EDX,0x28
		JB PassInjectReturn2

		XOR ECX,ECX
		MOV CL,BYTE PTR DS:[ESI]
		MOV AX,WORD PTR DS:[EDI]
		CMP AX,0x04
		JNZ PassInjectReturn1
		//39 from 0
		CMP CL,0x16
		JBE PassInjectNoMOV1
		MOV CL,0x16
PassInjectNoMOV1:
		ADD EDI,0x06
		MOV BYTE PTR DS:[EDI],CL
		ADD EDI,0x02
		ADD ESI,0x01
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
		MOV BYTE PTR DS:[EBX],0x00
			//66 from 0
PassInjectReturn1:
		CMP AX,0x0000
		JNZ PassInjectReturn2

		CMP CL,0x16
		JBE PassInjectNoMOV2
		MOV CL,0x16
PassInjectNoMOV2:
		ADD EDI,0x12
		MOV BYTE PTR DS:[EDI],CL
		ADD EDI,0x02
		ADD ESI,0x01
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
		MOV BYTE PTR DS:[EBX],0x00
		//103 from 0
PassInjectReturn2:
		POP EDI
		POP EDX
		POP ECX
		POP EBX
		POP EAX
		//JMP DontInject
		POP EAX
		POP ESI
		JMP InjectReturn
	}
}

void __declspec(naked) ASMFunction()
{
	_asm
	{
		PUSH ESI
		PUSH EAX
		ADD EAX,EDX
		MOV ESI,EAX
		//Old code
		MOV DWORD PTR SS:[EBP+0x10],EDX
		MOV EDX,DWORD PTR SS:[EBP+0x08]
		MOV DWORD PTR SS:[EBP+0x08],EDX

		//MOV OLDEcx,ECX
		//LEA ECX,Key
		//end Old Code
		LEA EAX,PasswordStore
		MOV BL,BYTE PTR DS:[EAX]
		CMP BL,0x01
		JNZ DontInject
		JMP ASMInjectPassword
DontInject:
		POP EAX
		POP ESI
		JMP InjectReturn
	}
}


void __declspec(naked) ASMPassGrab()
{
	//Code i overwrote
	_asm
	{
		MOV EBP,ESP
		SUB ESP,0x0A0
		PUSH EBX

	//8 from 0
		PUSH EAX
		PUSH ECX
		PUSH ESI
		LEA EDX,PasswordStore
		MOV BYTE PTR DS:[EDX],0x01
		ADD EDX,0x02
		MOV EAX,0x00

PassGrabLoop:
	//27 from 0
		MOV CX,WORD PTR DS:[EBX+EAX]
		PUSH EDX
		MOV DL,BYTE PTR DS:[EBX+EAX+0x2]
		CMP DL,0x00
		POP EDX
		JE ENDPassGrab
	
	//46 from 0
		MOV WORD PTR DS:[EAX+EDX],CX
		ADD EAX,0x02
		JMP PassGrabLoop
	//60 from 0
ENDPassGrab:
		MOV WORD PTR DS:[EAX+EDX],CX
		ADD EAX,0x02
		MOV BYTE PTR DS:[EDX-0x01],AL
		POP ESI
		POP ECX
		POP EAX
//74 from 0
		MOV EDX,EBX

		JMP PassJBackOff
	}
}

void GrabPWHook( BYTE* addr )
{
	DWORD dwOldProtection;
	VirtualProtect(addr, 5, PAGE_EXECUTE_READWRITE, &dwOldProtection);
	addr[0] = 0xE9;
	*((DWORD*)(addr + 1)) = (DWORD)((BYTE*)ASMPassGrab - addr) - 5;
	VirtualProtect(addr, 5, dwOldProtection, &dwOldProtection);

	PassJBackOff = addr + 0x09;
}

void InjectPWHook( BYTE* addr,bool disableencryption)
{
	DWORD dwOldProtection;
	VirtualProtect(addr, 5, PAGE_EXECUTE_READWRITE, &dwOldProtection);
	addr[0] = 0xE9;
	*((DWORD*)(addr + 1)) = (DWORD)((BYTE*)ASMFunction - addr) - 5;
	VirtualProtect(addr, 5, dwOldProtection, &dwOldProtection);

	/**
	* Disable Crypt
	**/
	if(disableencryption)
	{
		VirtualProtect(addr + 0x41, 3, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		addr[0x41] = 0x8A;
		addr[0x42] = 0x14;	// XOR DL,BYTE PTR DS:[EBX+EAX] to MOV DL,BYTE PTR DS:[EAX+EBX]
		addr[0x43]  = 0x18;
		VirtualProtect(addr + 0x41, 3, dwOldProtection, &dwOldProtection);
	}


	InjectReturn = addr + 0x09;
	PasswordStore = new unsigned char[100];
	ZeroMemory(PasswordStore,100);
}
