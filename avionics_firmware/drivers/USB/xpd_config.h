/* TODO Include the proper CMSIS device header */
//#include "include/stm32l4r5xx.h"

/* Replace type definitions */
//#define USB_TypeDef                __USB_TypeDef
//#define USB_OTG_INEndpointTypeDef  __USB_OTG_INEndpointTypeDef
//#define USB_OTG_OUTEndpointTypeDef __USB_OTG_OUTEndpointTypeDef
//#define USB_OTG_HostChannelTypeDef __USB_OTG_HostChannelTypeDef
#define USB_OTG_TypeDef            USB_OTG_DeviceTypeDef

#undef  USB
//#define USB ((__USB_TypeDef*)USB_BASE)
#define USB_OTG_FS ((USB_OTG_TypeDef)USB_OTG_FS_PERIPH_BASE)
/* TODO Copy USB peripheral registers structures from a compatible device header */
/* typedef of USB_TypeDef */
/* AND/OR */
/* typedef of USB_OTG_INEndpointTypeDef
   typedef of USB_OTG_OUTEndpointTypeDef
   typedef of USB_OTG_HostChannelTypeDef
   typedef of USB_OTG_TypeDef */

