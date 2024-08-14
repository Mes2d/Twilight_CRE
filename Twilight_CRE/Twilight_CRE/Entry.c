
/*

	MIT License

	Copyright (c) 2024 Byte Zone

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

*/

#include "includes.hpp"

const WCHAR sc_wszDeviceNameBuffer[] = L"\\Device\\Twilight_CRE";
const WCHAR sc_wszSymLinkBuffer[] = L"\\DosDevices\\Twilight_CRE";



NTSTATUS IRP_Read(PDEVICE_OBJECT pDriverObject, PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pDriverObject);

	char szBuffer[255] = { 0 };
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	strcpy(szBuffer, "Driver dispatched");
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}



NTSTATUS IRP_Write(PDEVICE_OBJECT pDriverObject, PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pDriverObject);

	char szBuffer[255] = { 0 };
	strcpy(szBuffer, pIrp->AssociatedIrp.SystemBuffer);
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = strlen(szBuffer);
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}



NTSTATUS Sys_Calls(PDEVICE_OBJECT pDriverObject, PIRP pIrp)
{
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	switch (pStack->MajorFunction)
	{
	case IRP_MJ_READ:
		IRP_Read(pDriverObject, pIrp);
		break;

	case IRP_MJ_WRITE:
		IRP_Write(pDriverObject, pIrp);
		break;

	default:
		pIrp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	}
	return STATUS_SUCCESS;
}



VOID Driver_Unload(IN PDRIVER_OBJECT pDrvObj)
{
	UNREFERENCED_PARAMETER(pDrvObj);
	UNICODE_STRING symLink;
	RtlInitUnicodeString(&symLink, sc_wszSymLinkBuffer);
	IoDeleteSymbolicLink(&symLink);
	if (pDrvObj && pDrvObj->DeviceObject)
	{
		IoDeleteDevice(pDrvObj->DeviceObject);
	}
}



NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath)
{

	UNREFERENCED_PARAMETER(pRegistryPath);

	pDriverObject->DriverUnload = &Driver_Unload;

	NTSTATUS status = 0;

	UNICODE_STRING deviceNameUnicodeString, deviceSymLinkUnicodeString;
	RtlInitUnicodeString(&deviceNameUnicodeString, sc_wszDeviceNameBuffer);
	RtlInitUnicodeString(&deviceSymLinkUnicodeString, sc_wszSymLinkBuffer);

	PDEVICE_OBJECT pDeviceObject = NULL;
	status = IoCreateDevice(pDriverObject, 0, &deviceNameUnicodeString, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	if (status != STATUS_SUCCESS)
	{
		Print("IoCreateDevice fail Status: %X\n", status); // https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-erref/596a1078-e883-4972-9bbc-49e60bebca55
		return status;
	}

	status = IoCreateSymbolicLink(&deviceSymLinkUnicodeString, &deviceNameUnicodeString);
	if (status != STATUS_SUCCESS)
	{
		Print("IoCreateSymbolicLink fail Status: %X\n", status);
		return status;
	}

	for (ULONG t = 0; t <= IRP_MJ_MAXIMUM_FUNCTION; t++)
		pDriverObject->MajorFunction[t] = &Sys_Calls;

	pDeviceObject->Flags |= DO_BUFFERED_IO;

	return STATUS_SUCCESS;
}