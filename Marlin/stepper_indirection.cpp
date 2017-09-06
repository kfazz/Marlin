/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * stepper_indirection.cpp
 *
 * Stepper motor driver indirection to allow some stepper functions to
 * be done via SPI/I2c instead of direct pin manipulation.
 *
 * Part of Marlin
 *
 * Copyright (c) 2015 Dominik Wenger
 */

#include "stepper_indirection.h"

#include "MarlinConfig.h"

//
// TMC26X Driver objects and inits
//
#if ENABLED(HAVE_TMCDRIVER)

  #include <SPI.h>
  #include <TMC26XStepper.h>

  #define _TMC_DEFINE(ST) TMC26XStepper stepper##ST(200, ST##_ENABLE_PIN, ST##_STEP_PIN, ST##_DIR_PIN, ST##_MAX_CURRENT, ST##_SENSE_RESISTOR)

  #if ENABLED(X_IS_TMC)
    _TMC_DEFINE(X);
  #endif
  #if ENABLED(X2_IS_TMC)
    _TMC_DEFINE(X2);
  #endif
  #if ENABLED(Y_IS_TMC)
    _TMC_DEFINE(Y);
  #endif
  #if ENABLED(Y2_IS_TMC)
    _TMC_DEFINE(Y2);
  #endif
  #if ENABLED(Z_IS_TMC)
    _TMC_DEFINE(Z);
  #endif
  #if ENABLED(Z2_IS_TMC)
    _TMC_DEFINE(Z2);
  #endif
  #if ENABLED(E0_IS_TMC)
    _TMC_DEFINE(E0);
  #endif
  #if ENABLED(E1_IS_TMC)
    _TMC_DEFINE(E1);
  #endif
  #if ENABLED(E2_IS_TMC)
    _TMC_DEFINE(E2);
  #endif
  #if ENABLED(E3_IS_TMC)
    _TMC_DEFINE(E3);
  #endif
  #if ENABLED(E4_IS_TMC)
    _TMC_DEFINE(E4);
  #endif

  #define _TMC_INIT(A) do{ \
    stepper##A.setMicrosteps(A##_MICROSTEPS); \
    stepper##A.start(); \
  }while(0)

  void tmc_init() {
    #if ENABLED(X_IS_TMC)
      _TMC_INIT(X);
    #endif
    #if ENABLED(X2_IS_TMC)
      _TMC_INIT(X2);
    #endif
    #if ENABLED(Y_IS_TMC)
      _TMC_INIT(Y);
    #endif
    #if ENABLED(Y2_IS_TMC)
      _TMC_INIT(Y2);
    #endif
    #if ENABLED(Z_IS_TMC)
      _TMC_INIT(Z);
    #endif
    #if ENABLED(Z2_IS_TMC)
      _TMC_INIT(Z2);
    #endif
    #if ENABLED(E0_IS_TMC)
      _TMC_INIT(E0);
    #endif
    #if ENABLED(E1_IS_TMC)
      _TMC_INIT(E1);
    #endif
    #if ENABLED(E2_IS_TMC)
      _TMC_INIT(E2);
    #endif
    #if ENABLED(E3_IS_TMC)
      _TMC_INIT(E3);
    #endif
    #if ENABLED(E4_IS_TMC)
      _TMC_INIT(E4);
    #endif
  }

#endif // HAVE_TMCDRIVER

//
// TMC2130 Driver objects and inits
//
#if ENABLED(HAVE_TMC2130)

  #include <SPI.h>
  #include <TMC2130Stepper.h>
  #include "enum.h"

  #define _TMC2130_DEFINE(ST) TMC2130Stepper stepper##ST(ST##_ENABLE_PIN, ST##_DIR_PIN, ST##_STEP_PIN, ST##_CS_PIN)

  // Stepper objects of TMC2130 steppers used
  #if ENABLED(X_IS_TMC2130)
    _TMC2130_DEFINE(X);
  #endif
  #if ENABLED(X2_IS_TMC2130)
    _TMC2130_DEFINE(X2);
  #endif
  #if ENABLED(Y_IS_TMC2130)
    _TMC2130_DEFINE(Y);
  #endif
  #if ENABLED(Y2_IS_TMC2130)
    _TMC2130_DEFINE(Y2);
  #endif
  #if ENABLED(Z_IS_TMC2130)
    _TMC2130_DEFINE(Z);
  #endif
  #if ENABLED(Z2_IS_TMC2130)
    _TMC2130_DEFINE(Z2);
  #endif
  #if ENABLED(E0_IS_TMC2130)
    _TMC2130_DEFINE(E0);
  #endif
  #if ENABLED(E1_IS_TMC2130)
    _TMC2130_DEFINE(E1);
  #endif
  #if ENABLED(E2_IS_TMC2130)
    _TMC2130_DEFINE(E2);
  #endif
  #if ENABLED(E3_IS_TMC2130)
    _TMC2130_DEFINE(E3);
  #endif
  #if ENABLED(E4_IS_TMC2130)
    _TMC2130_DEFINE(E4);
  #endif

  // Use internal reference voltage for current calculations. This is the default.
  // Following values from Trinamic's spreadsheet with values for a NEMA17 (42BYGHW609)
  // https://www.trinamic.com/products/integrated-circuits/details/tmc2130/
  void tmc2130_init(TMC2130Stepper &st, const uint16_t microsteps, const uint32_t thrs, const uint32_t spmm) {
    st.begin();
    st.setCurrent(st.getCurrent(), R_SENSE, HOLD_MULTIPLIER);
    st.microsteps(microsteps);
    st.blank_time(36);
    st.off_time(5); // Only enables the driver if used with stealthChop
    st.interpolate(INTERPOLATE);
    st.power_down_delay(128); // ~2s until driver lowers to hold current
    st.hysterisis_start(0); // HSTRT = 1
    st.hysterisis_low(1); // HEND = -2
    st.diag1_active_high(1); // For sensorless homing
    #if ENABLED(STEALTHCHOP)
      st.stealth_freq(1); // f_pwm = 2/683 f_clk
      st.stealth_autoscale(1);
      st.stealth_gradient(5);
      st.stealth_amplitude(255);
      st.stealthChop(1);
      #if ENABLED(HYBRID_THRESHOLD)
        st.stealth_max_speed(12650000UL*st.microsteps()/(256*thrs*spmm));
      #endif
    #elif ENABLED(SENSORLESS_HOMING)
      st.coolstep_min_speed(1024UL * 1024UL - 1UL);
    #endif
  }

  #define _TMC2130_INIT(ST, SPMM) tmc2130_init(stepper##ST, ST##_MICROSTEPS, ST##_HYBRID_THRESHOLD, SPMM)

  void tmc2130_init() {
    constexpr uint16_t steps_per_mm[] = DEFAULT_AXIS_STEPS_PER_UNIT;
    #if ENABLED(X_IS_TMC2130)
      _TMC2130_INIT( X, steps_per_mm[X_AXIS]);
      #if ENABLED(SENSORLESS_HOMING)
        stepperX.sg_stall_value(X_HOMING_SENSITIVITY);
      #endif
    #endif
    #if ENABLED(X2_IS_TMC2130)
      _TMC2130_INIT(X2, steps_per_mm[X_AXIS]);
    #endif
    #if ENABLED(Y_IS_TMC2130)
      _TMC2130_INIT( Y, steps_per_mm[Y_AXIS]);
      #if ENABLED(SENSORLESS_HOMING)
        stepperY.sg_stall_value(Y_HOMING_SENSITIVITY);
      #endif
    #endif
    #if ENABLED(Y2_IS_TMC2130)
      _TMC2130_INIT(Y2, steps_per_mm[Y_AXIS]);
    #endif
    #if ENABLED(Z_IS_TMC2130)
      _TMC2130_INIT( Z, steps_per_mm[Z_AXIS]);
    #endif
    #if ENABLED(Z2_IS_TMC2130)
      _TMC2130_INIT(Z2, steps_per_mm[Z_AXIS]);
    #endif
    #if ENABLED(E0_IS_TMC2130)
      _TMC2130_INIT(E0, steps_per_mm[E_AXIS]);
    #endif
    #if ENABLED(E1_IS_TMC2130)
      { constexpr int extruder = 1; _TMC2130_INIT(E1, steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E2_IS_TMC2130)
      { constexpr int extruder = 2; _TMC2130_INIT(E2, steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E3_IS_TMC2130)
      { constexpr int extruder = 3; _TMC2130_INIT(E3, steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E4_IS_TMC2130)
      { constexpr int extruder = 4; _TMC2130_INIT(E4, steps_per_mm[E_AXIS_N]); }
    #endif

    TMC2130_ADV()
  }
#endif // HAVE_TMC2130

//
// TMC2208 Driver objects and inits
//
#if ENABLED(HAVE_TMC2208)

#if ENABLED(IS_REARM)
    #include "src/HAL/HAL_LPC1768/SoftwareSerial.h"
    #include "src/HAL/HAL_LPC1768/HardwareSerial.h"
    #include "src/HAL/HAL_LPC1768/TMC2208Stepper.h"
    extern HardwareSerial Serial3;
  #else
    #include <SoftwareSerial.h>
    #include <HardwareSerial.h>
    #include <TMC2208Stepper.h>
  #endif
  #include "serial.h"

  #define _TMC2208_DEFINE_HARDWARE(ST) TMC2208Stepper stepper##ST(&ST##_HARDWARE_SERIAL)
   #define _TMC2208_DEFINE_SOFTWARE(ST) SoftwareSerial stepper##ST##_serial = SoftwareSerial(ST##_SERIAL_RX_PIN, ST##_SERIAL_TX_PIN); \
                                        TMC2208Stepper stepper##ST(&stepper##ST##_serial, ST##_SERIAL_RX_PIN > -1)

   // Stepper objects of TMC2208 steppers used
   #if ENABLED(X_IS_TMC2208)
     #if defined(X_HARDWARE_SERIAL)
       _TMC2208_DEFINE_HARDWARE(X);
     #else
       _TMC2208_DEFINE_SOFTWARE(X);
     #endif
   #endif
   #if ENABLED(X2_IS_TMC2208)
     #if defined(X2_HARDWARE_SERIAL)
       _TMC2208_DEFINE_HARDWARE(X2);
     #else
       _TMC2208_DEFINE_SOFTWARE(X2);
     #endif
   #endif
   #if ENABLED(Y_IS_TMC2208)
     #if defined(Y_HARDWARE_SERIAL)
       _TMC2208_DEFINE_HARDWARE(Y);
     #else
       _TMC2208_DEFINE_SOFTWARE(Y);
     #endif
   #endif
   #if ENABLED(Y2_IS_TMC2208)
     #if defined(Y2_HARDWARE_SERIAL)
       _TMC2208_DEFINE_HARDWARE(Y2);
     #else
       _TMC2208_DEFINE_SOFTWARE(Y2);
     #endif
   #endif
   #if ENABLED(Z_IS_TMC2208)
     #if defined(Z_HARDWARE_SERIAL)
       _TMC2208_DEFINE_HARDWARE(Z);
     #else
       _TMC2208_DEFINE_SOFTWARE(Z);
     #endif
   #endif
   #if ENABLED(Z2_IS_TMC2208)
     #if defined(Z2_HARDWARE_SERIAL)
       _TMC2208_DEFINE_HARDWARE(Z2);
     #else
       _TMC2208_DEFINE_SOFTWARE(Z2);
     #endif
   #endif
   #if ENABLED(E0_IS_TMC2208)
     #if defined(E0_HARDWARE_SERIAL)
       _TMC2208_DEFINE_HARDWARE(E0);
     #else
       _TMC2208_DEFINE_SOFTWARE(E0);
     #endif
   #endif
   #if ENABLED(E1_IS_TMC2208)
     #if defined(E1_HARDWARE_SERIAL)
       _TMC2208_DEFINE_HARDWARE(E1);
     #else
       _TMC2208_DEFINE_SOFTWARE(E1);
     #endif
   #endif
   #if ENABLED(E2_IS_TMC2208)
     #if defined(E2_HARDWARE_SERIAL)
       _TMC2208_DEFINE_HARDWARE(E2);
     #else
       _TMC2208_DEFINE_SOFTWARE(E2);
     #endif
   #endif
   #if ENABLED(E3_IS_TMC2208)
     #if defined(E3_HARDWARE_SERIAL)
       _TMC2208_DEFINE_HARDWARE(E3);
     #else
       _TMC2208_DEFINE_SOFTWARE(E3);
     #endif
   #endif
   #if ENABLED(E4_IS_TMC2208)
     #if defined(E4_HARDWARE_SERIAL)
       _TMC2208_DEFINE_HARDWARE(E4);
     #else
       _TMC2208_DEFINE_SOFTWARE(E4);
     #endif
   #endif

#define  _TMC2208_SOFTWARE_BEGIN(ST) stepper##ST##_serial.begin(115200);

   void tmc2208_serial_begin() {
     #if ENABLED(X_IS_TMC2208)
       #if defined(X_HARDWARE_SERIAL)
         X_HARDWARE_SERIAL.begin(250000);
       #else
         _TMC2208_SOFTWARE_BEGIN(X);
       #endif
     #endif
     #if ENABLED(X2_IS_TMC2208)
       #if defined(X2_HARDWARE_SERIAL)
         X2_HARDWARE_SERIAL.begin(250000);
       #else
         _TMC2208_SOFTWARE_BEGIN(X2);
       #endif
     #endif
     #if ENABLED(Y_IS_TMC2208)
       #if defined(Y_HARDWARE_SERIAL)
         Y_HARDWARE_SERIAL.begin(250000);
       #else
         _TMC2208_SOFTWARE_BEGIN(Y);
       #endif
     #endif
     #if ENABLED(Y2_IS_TMC2208)
       #if defined(Y2_HARDWARE_SERIAL)
         Y2_HARDWARE_SERIAL.begin(250000);
       #else
         _TMC2208_SOFTWARE_BEGIN(Y2);
       #endif
     #endif
     #if ENABLED(Z_IS_TMC2208)
       #if defined(Z_HARDWARE_SERIAL)
         Z_HARDWARE_SERIAL.begin(250000);
       #else
         _TMC2208_SOFTWARE_BEGIN(Z);
       #endif
     #endif
     #if ENABLED(Z2_IS_TMC2208)
       #if defined(Z2_HARDWARE_SERIAL)
         Z2_HARDWARE_SERIAL.begin(250000);
       #else
         _TMC2208_SOFTWARE_BEGIN(Z2);
       #endif
     #endif
     #if ENABLED(E0_IS_TMC2208)
       #if defined(E0_HARDWARE_SERIAL)
         E0_HARDWARE_SERIAL.begin(250000);
       #else
         _TMC2208_SOFTWARE_BEGIN(E0);
       #endif
     #endif
     #if ENABLED(E1_IS_TMC2208)
       #if defined(E1_HARDWARE_SERIAL)
         E1_HARDWARE_SERIAL.begin(250000);
       #else
         _TMC2208_SOFTWARE_BEGIN(E1);
       #endif
     #endif
     #if ENABLED(E2_IS_TMC2208)
       #if defined(E2_HARDWARE_SERIAL)
         E2_HARDWARE_SERIAL.begin(250000);
       #else
         _TMC2208_SOFTWARE_BEGIN(E2);
       #endif
     #endif
     #if ENABLED(E3_IS_TMC2208)
       #if defined(E3_HARDWARE_SERIAL)
         E3_HARDWARE_SERIAL.begin(250000);
       #else
         _TMC2208_SOFTWARE_BEGIN(E3);
       #endif
     #endif
     #if ENABLED(E4_IS_TMC2208)
       #if defined(E4_HARDWARE_SERIAL)
         E4_HARDWARE_SERIAL.begin(250000);
       #else
         _TMC2208_SOFTWARE_BEGIN(E4);
       #endif
     #endif
   }
  // Use internal reference voltage for current calculations. This is the default.
  // Following values from Trinamic's spreadsheet with values for a NEMA17 (42BYGHW609)
  void tmc2208_init(TMC2208Stepper &st, const uint16_t microsteps) {
    uint32_t data;
    bool res = false;
    st.pdn_disable(true); // Use UART
    //st.senddelay(1);
    res = st.GCONF(&data);
    SERIAL_ECHOPAIR(" Res:",  res);
    SERIAL_ECHOPAIR(" Initial GCONF:",  data);
  //  res = st.OTP_READ(&data);
  //  SERIAL_ECHOPAIR(" OTP_READ:",  data);
    res = st.IOIN(&data);
    SERIAL_ECHOPAIR(" Res:",  res);
    SERIAL_ECHOPAIR(" IOIN:",  data);
    res = st.FACTORY_CONF(&data);
    SERIAL_ECHOPAIR(" Res:",  res);
    SERIAL_ECHOPAIR(" FACTORY_CONF:",  data);

    st.mstep_reg_select(true); // Select microsteps with UART
    st.I_scale_analog(false);
    st.rms_current(st.getCurrent(), HOLD_MULTIPLIER, R_SENSE);
    st.microsteps(microsteps);
    st.tbl(0b10); // Blank time = 24
    st.toff(0b10); // Off time = 2
    st.intpol(INTERPOLATE);
    #if DISABLED(STEALTHCHOP)
      st.en_spreadCycle(true);
    #endif
    res = st.GCONF(&data);
    SERIAL_ECHOPAIR(" Res:",  res);
    SERIAL_ECHOPAIR(" Final GCONF:",  data);
  }

    void tmc2208_adv(TMC2208Stepper &st) {
      st.en_spreadCycle(true);
    }

#define _TMC2208_INIT(ST) tmc2208_init(stepper##ST, ST##_MICROSTEPS)
#define _TMC2208_ADV(ST) tmc2208_adv(stepper##ST)
  void tmc2208_init() {
    #if ENABLED(X_IS_TMC2208)
      _TMC2208_INIT(X);
    #endif
    #if ENABLED(X2_IS_TMC2208)
      _TMC2208_INIT(X2);
    #endif
    #if ENABLED(Y_IS_TMC2208)
      _TMC2208_INIT(Y);
    #endif
    #if ENABLED(Y2_IS_TMC2208)
      _TMC2208_INIT(Y2);
    #endif
    #if ENABLED(Z_IS_TMC2208)
      _TMC2208_INIT(Z);
    #endif
    #if ENABLED(Z2_IS_TMC2208)
      _TMC2208_INIT(Z2);
    #endif
    #if ENABLED(E0_IS_TMC2208)
      _TMC2208_INIT(E0);
      _TMC2208_ADV(E0);
    #endif
    #if ENABLED(E1_IS_TMC2208)
      { constexpr int extruder = 1; _TMC2208_INIT(E1, steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E2_IS_TMC2208)
      { constexpr int extruder = 2; _TMC2208_INIT(E2, steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E3_IS_TMC2208)
      { constexpr int extruder = 3; _TMC2208_INIT(E3, steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E4_IS_TMC2208)
      { constexpr int extruder = 4; _TMC2208_INIT(E4, steps_per_mm[E_AXIS_N]); }
    #endif

    TMC2208_ADV()
  }
#endif // HAVE_TMC2208

//
// L6470 Driver objects and inits
//
#if ENABLED(HAVE_L6470DRIVER)

  #include <SPI.h>
  #include <L6470.h>

  #define _L6470_DEFINE(ST) L6470 stepper##ST(ST##_ENABLE_PIN)

  // L6470 Stepper objects
  #if ENABLED(X_IS_L6470)
    _L6470_DEFINE(X);
  #endif
  #if ENABLED(X2_IS_L6470)
    _L6470_DEFINE(X2);
  #endif
  #if ENABLED(Y_IS_L6470)
    _L6470_DEFINE(Y);
  #endif
  #if ENABLED(Y2_IS_L6470)
    _L6470_DEFINE(Y2);
  #endif
  #if ENABLED(Z_IS_L6470)
    _L6470_DEFINE(Z);
  #endif
  #if ENABLED(Z2_IS_L6470)
    _L6470_DEFINE(Z2);
  #endif
  #if ENABLED(E0_IS_L6470)
    _L6470_DEFINE(E0);
  #endif
  #if ENABLED(E1_IS_L6470)
    _L6470_DEFINE(E1);
  #endif
  #if ENABLED(E2_IS_L6470)
    _L6470_DEFINE(E2);
  #endif
  #if ENABLED(E3_IS_L6470)
    _L6470_DEFINE(E3);
  #endif
  #if ENABLED(E4_IS_L6470)
    _L6470_DEFINE(E4);
  #endif

  #define _L6470_INIT(A) do{ \
    stepper##A.init(A##_K_VAL); \
    stepper##A.softFree(); \
    stepper##A.setMicroSteps(A##_MICROSTEPS); \
    stepper##A.setOverCurrent(A##_OVERCURRENT); \
    stepper##A.setStallCurrent(A##_STALLCURRENT); \
  }while(0)

  void L6470_init() {
    #if ENABLED(X_IS_L6470)
      _L6470_INIT(X);
    #endif
    #if ENABLED(X2_IS_L6470)
      _L6470_INIT(X2);
    #endif
    #if ENABLED(Y_IS_L6470)
      _L6470_INIT(Y);
    #endif
    #if ENABLED(Y2_IS_L6470)
      _L6470_INIT(Y2);
    #endif
    #if ENABLED(Z_IS_L6470)
      _L6470_INIT(Z);
    #endif
    #if ENABLED(Z2_IS_L6470)
      _L6470_INIT(Z2);
    #endif
    #if ENABLED(E0_IS_L6470)
      _L6470_INIT(E0);
    #endif
    #if ENABLED(E1_IS_L6470)
      _L6470_INIT(E1);
    #endif
    #if ENABLED(E2_IS_L6470)
      _L6470_INIT(E2);
    #endif
    #if ENABLED(E3_IS_L6470)
      _L6470_INIT(E3);
    #endif
    #if ENABLED(E4_IS_L6470)
      _L6470_INIT(E4);
    #endif
  }

#endif // HAVE_L6470DRIVER
