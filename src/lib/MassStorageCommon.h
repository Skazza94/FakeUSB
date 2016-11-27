/*
  Copyright 2013  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.

  Edited by Skazza
 */
#ifndef USBPROXY_MS_COMMON_H
#define USBPROXY_MS_COMMON_H

/* Macros: */
/** Magic signature for a Command Block Wrapper used in the Mass Storage Bulk-Only transport protocol. */
#define MS_CBW_SIGNATURE                               0x43425355UL

/** Magic signature for a Command Status Wrapper used in the Mass Storage Bulk-Only transport protocol. */
#define MS_CSW_SIGNATURE                               0x53425355UL

/** Mask for a Command Block Wrapper's flags attribute to specify a command with data sent from host-to-device. */
#define MS_COMMAND_DIR_DATA_OUT                        (0 << 7)

/** Mask for a Command Block Wrapper's flags attribute to specify a command with data sent from device-to-host. */
#define MS_COMMAND_DIR_DATA_IN                         (1 << 7)

/** CSW Packet Size */
#define MS_CSW_SIZE										0x0d

/** CBW Packet Size */
#define MS_CBW_SIZE										0x1f

/** SCSI Command Code for an INQUIRY command. */
#define SCSI_CMD_INQUIRY                               0x12

/** SCSI Command Code for a REQUEST SENSE command. */
#define SCSI_CMD_REQUEST_SENSE                         0x03

/** SCSI Command Code for a START STOP UNIT command. */
#define SCSI_CMD_START_STOP_UNIT					   0x1b

/** SCSI Command Code for a TEST UNIT READY command. */
#define SCSI_CMD_TEST_UNIT_READY                       0x00

/** SCSI Command Code for a READ CAPACITY (10) command. */
#define SCSI_CMD_READ_CAPACITY_10                      0x25

/** SCSI Command Code for a SEND DIAGNOSTIC command. */
#define SCSI_CMD_SEND_DIAGNOSTIC                       0x1D

/** SCSI Command Code for a PREVENT ALLOW MEDIUM REMOVAL command. */
#define SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL          0x1E

/** SCSI Command Code for a WRITE (10) command. */
#define SCSI_CMD_WRITE_10                              0x2A

/** SCSI Command Code for a READ (10) command. */
#define SCSI_CMD_READ_10                               0x28

/** SCSI Command Code for a WRITE (6) command. */
#define SCSI_CMD_WRITE_6                               0x0A

/** SCSI Command Code for a READ (6) command. */
#define SCSI_CMD_READ_6                                0x08

/** SCSI Command Code for a VERIFY (10) command. */
#define SCSI_CMD_VERIFY_10                             0x2F

/** SCSI Command Code for a MODE SENSE (6) command. */
#define SCSI_CMD_MODE_SENSE_6                          0x1A

/** SCSI Command Code for a MODE SENSE (10) command. */
#define SCSI_CMD_MODE_SENSE_10                         0x5A

/** SCSI Sense Code to indicate no error has occurred. */
#define SCSI_SENSE_KEY_GOOD                            0x00

/** SCSI Sense Code to indicate that the device has recovered from an error. */
#define SCSI_SENSE_KEY_RECOVERED_ERROR                 0x01

/** SCSI Sense Code to indicate that the device is not ready for a new command. */
#define SCSI_SENSE_KEY_NOT_READY                       0x02

/** SCSI Sense Code to indicate an error whilst accessing the medium. */
#define SCSI_SENSE_KEY_MEDIUM_ERROR                    0x03

/** SCSI Sense Code to indicate a hardware error has occurred. */
#define SCSI_SENSE_KEY_HARDWARE_ERROR                  0x04

/** SCSI Sense Code to indicate that an illegal request has been issued. */
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST                 0x05

/** SCSI Sense Code to indicate that the unit requires attention from the host to indicate
 *  a reset event, medium removal or other condition.
 */
#define SCSI_SENSE_KEY_UNIT_ATTENTION                  0x06

/** SCSI Sense Code to indicate that a write attempt on a protected block has been made. */
#define SCSI_SENSE_KEY_DATA_PROTECT                    0x07

/** SCSI Sense Code to indicate an error while trying to write to a write-once medium. */
#define SCSI_SENSE_KEY_BLANK_CHECK                     0x08

/** SCSI Sense Code to indicate a vendor specific error has occurred. */
#define SCSI_SENSE_KEY_VENDOR_SPECIFIC                 0x09

/** SCSI Sense Code to indicate that an EXTENDED COPY command has aborted due to an error. */
#define SCSI_SENSE_KEY_COPY_ABORTED                    0x0A

/** SCSI Sense Code to indicate that the device has aborted the issued command. */
#define SCSI_SENSE_KEY_ABORTED_COMMAND                 0x0B

/** SCSI Sense Code to indicate an attempt to write past the end of a partition has been made. */
#define SCSI_SENSE_KEY_VOLUME_OVERFLOW                 0x0D

/** SCSI Sense Code to indicate that the source data did not match the data read from the medium. */
#define SCSI_SENSE_KEY_MISCOMPARE                      0x0E

/* Enums: */
/** Enum for the Mass Storage class specific control requests that can be issued by the USB bus host. */
enum MS_ClassRequests_t
{
	MS_REQ_GetMaxLUN                  = 0xFE, /**< Mass Storage class-specific request to retrieve the total number of Logical
	 *   Units (drives) in the SCSI device.
	 */
	MS_REQ_MassStorageReset           = 0xFF, /**< Mass Storage class-specific request to reset the Mass Storage interface,
	 *   ready for the next command.
	 */
};

/** Enum for the possible command status wrapper return status codes. */
enum MS_CommandStatusCodes_t
{
	MS_SCSI_COMMAND_Pass              = 0, /**< Command completed with no error */
	MS_SCSI_COMMAND_Fail              = 1, /**< Command failed to complete - host may check the exact error via a
	 *   SCSI REQUEST SENSE command.
	 */
	MS_SCSI_COMMAND_PhaseError        = 2, /**< Command failed due to being invalid in the current phase. */
};

/* Type Defines: */
/** Mass Storage Class Command Block Wrapper.
 *
 *  Type define for a Command Block Wrapper, used in the Mass Storage Bulk-Only Transport protocol.
 *
 *  Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct
{
	uint32_t Signature; /**< Command block signature, must be \ref MS_CBW_SIGNATURE to indicate a valid Command Block. */
	uint32_t Tag; /**< Unique command ID value, to associate a command block wrapper with its command status wrapper. */
	uint32_t DataTransferLength; /**< Length of the optional data portion of the issued command, in bytes. */
	uint8_t  Flags; /**< Command block flags, indicating command data direction. */
	uint8_t  LUN; /**< Logical Unit number this command is issued to. */
	uint8_t  SCSICommandLength; /**< Length of the issued SCSI command within the SCSI command data array. */
	uint8_t  SCSICommandData[16]; /**< Issued SCSI command in the Command Block. */
} MS_CommandBlockWrapper_t;

/** Mass Storage Class Command Status Wrapper.
 *
 *  Type define for a Command Status Wrapper, used in the Mass Storage Bulk-Only Transport protocol.
 *
 *  Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct
{
	uint32_t Signature; /**< Status block signature, must be \ref MS_CSW_SIGNATURE to indicate a valid Command Status. */
	uint32_t Tag; /**< Unique command ID value, to associate a command block wrapper with its command status wrapper. */
	uint32_t DataTransferResidue; /**< Number of bytes of data not processed in the SCSI command. */
	uint8_t  Status; /**< Status code of the issued command - a value from the \ref MS_CommandStatusCodes_t enum. */
} MS_CommandStatusWrapper_t;

/** Mass Storage Class SCSI Sense Structure
 *
 *  Type define for a SCSI Sense structure. Structures of this type are filled out by the
 *  device via the \ref MS_Host_RequestSense() function, indicating the current sense data of the
 *  device (giving explicit error codes for the last issued command). For details of the
 *  structure contents, refer to the SCSI specifications.
 */
typedef struct
{
	uint8_t  ResponseCode;

	uint8_t  SegmentNumber;

	unsigned SenseKey            : 4;
	unsigned Reserved            : 1;
	unsigned ILI                 : 1;
	unsigned EOM                 : 1;
	unsigned FileMark            : 1;

	uint8_t  Information[4];
	uint8_t  AdditionalLength;
	uint8_t  CmdSpecificInformation[4];
	uint8_t  AdditionalSenseCode;
	uint8_t  AdditionalSenseQualifier;
	uint8_t  FieldReplaceableUnitCode;
	uint8_t  SenseKeySpecific[3];
} SCSI_Request_Sense_Response_t;

/** Mass Storage Class SCSI Inquiry Structure.
 *
 *  Type define for a SCSI Inquiry structure. Structures of this type are filled out by the
 *  device via the \ref MS_Host_GetInquiryData() function, retrieving the attached device's
 *  information.
 *
 *  For details of the structure contents, refer to the SCSI specifications.
 */
typedef struct
{
	unsigned DeviceType          : 5;
	unsigned PeripheralQualifier : 3;

	unsigned Reserved            : 7;
	unsigned Removable           : 1;

	uint8_t  Version;

	unsigned ResponseDataFormat  : 4;
	unsigned Reserved2           : 1;
	unsigned NormACA             : 1;
	unsigned TrmTsk              : 1;
	unsigned AERC                : 1;

	uint8_t  AdditionalLength;
	uint8_t  Reserved3[2];

	unsigned SoftReset           : 1;
	unsigned CmdQue              : 1;
	unsigned Reserved4           : 1;
	unsigned Linked              : 1;
	unsigned Sync                : 1;
	unsigned WideBus16Bit        : 1;
	unsigned WideBus32Bit        : 1;
	unsigned RelAddr             : 1;

	uint8_t  VendorID[8];
	uint8_t  ProductID[16];
	uint8_t  RevisionID[4];
} SCSI_Inquiry_Response_t;

#endif /* USBPROXY_MS_COMMON_H */

