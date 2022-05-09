#include "xaxidma.h"
#include "xparameters.h"
#include "sleep.h"
#include "xil_cache.h"
#include "xio.h"
u32 checkHalted(u32 baseAddr, u32 offset);

int main()
{
//	u32 a[]={1,2,3,4,5,6,7,8};
//	u32 b[8];
	u32 a = XPAR_BRAM_0_BASEADDR;
	u32 b = XPAR_BRAM_0_BASEADDR + 8*sizeof(u32);

	for(int i=0;i<8;i++)
	{
		XIo_Out32(a,i+1);
		a += 4;
	}
	a = XPAR_BRAM_0_BASEADDR;
	u32 status;
	XAxiDma_Config * myDmaConfig;
	XAxiDma myDma;
	myDmaConfig = XAxiDma_LookupConfigBaseAddr(XPAR_AXI_DMA_0_BASEADDR);
	status = XAxiDma_CfgInitialize(&myDma,myDmaConfig);
	if(status != XST_SUCCESS)
	{
		xil_printf("DMA Init Failed\n");
		return -1;
	}
	xil_printf("DMA Init Success\n");
	//xil_printf("D.I.S\n");

	status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x4);
	xil_printf("Status before data transfer %d\n",status);
	//xil_printf("S.B.T %d\n",status);

	//	Xil_DCacheFlushRange((u32)a,8*sizeof(u32)); //PS part
	status = XAxiDma_SimpleTransfer(&myDma,(u32)a,8*sizeof(u32),XAXIDMA_DMA_TO_DEVICE);
	status = XAxiDma_SimpleTransfer(&myDma,(u32)b,8*sizeof(u32),XAXIDMA_DEVICE_TO_DMA);
	if(status != XST_SUCCESS)
	{
		xil_printf("DMA Transfer Failed\n");
		return -1;
	}
	status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x4);
	while(status != 1)
	{
		status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x4);
	}
	status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x34);
	while(status != 1)
	{
		status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x34);
	}
//	while ((XAxiDma_Busy(&myDma,XAXIDMA_DEVICE_TO_DMA)) ||
//		(XAxiDma_Busy(&myDma,XAXIDMA_DMA_TO_DEVICE))) {
//		xil_printf("w.\n");
//	}
	xil_printf("DMA Transfer Succcess\n");
	//xil_printf("D.T.S\n");
	for(int i=0;i<8;i++)
	{
		xil_printf("%0X\n",XIo_In32(b));
		b+=4;
	}
}
u32 checkHalted(u32 baseAddr,u32 offset)
{
	u32 status;
	status = XAxiDma_ReadReg(baseAddr,offset) & XAXIDMA_HALTED_MASK;
	return status;
}
