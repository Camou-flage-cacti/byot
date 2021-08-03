
/*******************************************************************
*
* CAUTION: This file is automatically generated by HSI.
* Version: 
* DO NOT EDIT.
*
* Copyright (C) 2010-2021 Xilinx, Inc. All Rights Reserved.*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the Software), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in
*all copies or substantial portions of the Software.
* 
* Use of the Software is limited solely to applications:
*(a) running on a Xilinx device, or
*(b) that interact with a Xilinx device through a bus or interconnect.
*
*THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
*XILINX BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
*OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*Except as contained in this notice, the name of the Xilinx shall not be used
*in advertising or otherwise to promote the sale, use or other dealings in
*this Software without prior written authorization from Xilinx.
*

* 
* Description: Driver configuration
*
*******************************************************************/

#include "xparameters.h"
#include "xbram.h"

/*
* The configuration table for devices
*/

XBram_Config XBram_ConfigTable[] =
{
	{
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_DEVICE_ID,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_DATA_WIDTH,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_ECC,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_FAULT_INJECT,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_CE_FAILING_REGISTERS,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_CE_FAILING_REGISTERS,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_UE_FAILING_REGISTERS,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_UE_FAILING_REGISTERS,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_ECC_STATUS_REGISTERS,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_CE_COUNTER_WIDTH,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_ECC_ONOFF_REGISTER,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_ECC_ONOFF_RESET_VALUE,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_WRITE_ACCESS,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_BASEADDR,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_HIGHADDR,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_S_AXI_CTRL_BASEADDR,
		XPAR_DATA_LMB_BRAM_IF_CNTLR_1_S_AXI_CTRL_HIGHADDR
	},
	{
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_DEVICE_ID,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_DATA_WIDTH,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_ECC,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_FAULT_INJECT,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_CE_FAILING_REGISTERS,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_CE_FAILING_REGISTERS,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_UE_FAILING_REGISTERS,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_UE_FAILING_REGISTERS,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_ECC_STATUS_REGISTERS,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_CE_COUNTER_WIDTH,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_ECC_ONOFF_REGISTER,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_ECC_ONOFF_RESET_VALUE,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_WRITE_ACCESS,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_BASEADDR,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_HIGHADDR,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_S_AXI_CTRL_BASEADDR,
		XPAR_INS_LMB_BRAM_IF_CNTLR_0_S_AXI_CTRL_HIGHADDR
	},
	{
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_DEVICE_ID,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_DATA_WIDTH,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_ECC,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_FAULT_INJECT,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_CE_FAILING_REGISTERS,
		0,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_UE_FAILING_REGISTERS,
		0,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_ECC_STATUS_REGISTERS,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_CE_COUNTER_WIDTH,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_ECC_ONOFF_REGISTER,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_ECC_ONOFF_RESET_VALUE,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_WRITE_ACCESS,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_S_AXI_BASEADDR,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_S_AXI_HIGHADDR,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_S_AXI_CTRL_BASEADDR,
		XPAR_MB_DMA_AXI_BRAM_CTRL_0_S_AXI_CTRL_HIGHADDR
	},
	{
		XPAR_SHARE_AXI_BRAM_CTRL_0_DEVICE_ID,
		XPAR_SHARE_AXI_BRAM_CTRL_0_DATA_WIDTH,
		XPAR_SHARE_AXI_BRAM_CTRL_0_ECC,
		XPAR_SHARE_AXI_BRAM_CTRL_0_FAULT_INJECT,
		XPAR_SHARE_AXI_BRAM_CTRL_0_CE_FAILING_REGISTERS,
		0,
		XPAR_SHARE_AXI_BRAM_CTRL_0_UE_FAILING_REGISTERS,
		0,
		XPAR_SHARE_AXI_BRAM_CTRL_0_ECC_STATUS_REGISTERS,
		XPAR_SHARE_AXI_BRAM_CTRL_0_CE_COUNTER_WIDTH,
		XPAR_SHARE_AXI_BRAM_CTRL_0_ECC_ONOFF_REGISTER,
		XPAR_SHARE_AXI_BRAM_CTRL_0_ECC_ONOFF_RESET_VALUE,
		XPAR_SHARE_AXI_BRAM_CTRL_0_WRITE_ACCESS,
		XPAR_SHARE_AXI_BRAM_CTRL_0_S_AXI_BASEADDR,
		XPAR_SHARE_AXI_BRAM_CTRL_0_S_AXI_HIGHADDR,
		XPAR_SHARE_AXI_BRAM_CTRL_0_S_AXI_CTRL_BASEADDR,
		XPAR_SHARE_AXI_BRAM_CTRL_0_S_AXI_CTRL_HIGHADDR
	}
};


