from distutils.core import setup, Extension

tbi_module = Extension('_tbi',
                    sources = ['tbi_wrap.cxx', 'tbi_core.cpp', 'tbi_device.cpp', 'tbi_service.cpp', 'attribute.cpp', 'tbit.cpp', 'hid.c', 'basicdemo.cpp', 'chopper.cpp', 'dmm.cpp', 'tbi_device_manager.cpp', 'adc.cpp', 'adc_hw.cpp', 'gpio_hw.cpp', 'i2c_hw.cpp', 'pin.cpp'],
                    libraries=['setupapi'],
                )

setup (name = 'tbi',
       version = '1.0',
       description = 'This is a tbi package',
       ext_modules = [tbi_module],
       py_modules = ["tbi"],)
