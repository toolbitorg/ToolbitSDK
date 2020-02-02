%module toolbit
%{
#include "tbi_core.h"
#include "tbi_device.h"
#include "tbi_device_manager.h"
#include "tbi_service.h"
#include "attribute.h"
#include "tbit.h"
#include "hidapi.h"
#include "basicdemo.h"
#include "chopper.h"
#include "dmm.h"
#include "adc.h"
#include "adc_hw.h"
#include "gpio_hw.h"
#include "i2c_hw.h"
#include "pin.h"
%}
%include <stdint.i>
%include <std_string.i>
%inline %{
using namespace std;
%}
%include "tbi_core.h"
%include "tbi_device.h"
%include "tbi_device_manager.h"
%include "tbi_service.h"
%include "attribute.h"
%include "tbit.h"
%include "hidapi.h"
%include "basicdemo.h"
%include "chopper.h"
%include "dmm.h"
%include "adc.h"
%include "adc_hw.h"
%include "gpio_hw.h"
%include "i2c_hw.h"
%include "pin.h"
