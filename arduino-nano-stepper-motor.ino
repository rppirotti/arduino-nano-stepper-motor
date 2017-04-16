/*
 * Copyright 2017 Rodolfo Pirotti
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
 */

#include <TimerOne.h>

/// Some help for Arduino NANO data types
//Byte: 1
//char: 1
//short: 2
//int: 2
//long: 4
//long long: 8
//bool: 1
//boolean: 1
//float: 4
//double: 4
//char*: 2
//int*: 2
//long*: 2
//float*: 2
//double*: 2
//void*: 2

///////////////////////////////////////////////////////////////////////////////
/// @brief Data types definition
///////////////////////////////////////////////////////////////////////////////
typedef  char                 int8;
typedef  unsigned char        uint8;
//typedef  short                int16;
//typedef  unsigned short       uint16;
typedef  int                  int16;
typedef  unsigned int         uint16;
typedef  long                 int32;
typedef  unsigned long        uint32;
typedef  long long            int64;
typedef  unsigned long long   uint64;

///////////////////////////////////////////////////////////////////////////////
/// @class StepperMotor
/// @brief This class is used to control one stepper motor
/// @details It was designed and tested on an Arduino NANO with 28BYJ48
///          stepper motor.
///////////////////////////////////////////////////////////////////////////////
class StepperMotor
{
public:

  /////////////////////////////////////////////////////////////////////////////
  /// @brief Ctor
  /// @param numberOfStepsFullRotation Number of steps for a full rotation on the motor
  /// @param pin1 The pin connected to the first  coil (+)
  /// @param pin2 The pin connected to the second coil (+)
  /// @param pin3 The pin connected to the first  coil (-)
  /// @param pin4 The pin connected to the second coil (-)
  /////////////////////////////////////////////////////////////////////////////
  StepperMotor ( uint16 numberOfStepsFullRotation, uint16 pin1, uint16 pin2, uint16 pin3, uint16 pin4 )
  : m_numberOfStepsFullRotation ( numberOfStepsFullRotation )
  , m_pin1 ( pin1 )
  , m_pin2 ( pin2 )
  , m_pin3 ( pin3 )
  , m_pin4 ( pin4 )
  , m_isHalfStep ( false )
  , m_speed ( 10 )
  , m_delayBetweenSteps ( 1 )
  , m_state ( 0 )
  , m_isClockwise ( false )
  {
    digitalWrite ( m_pin1, LOW );
    digitalWrite ( m_pin2, LOW );
    digitalWrite ( m_pin3, LOW );
    digitalWrite ( m_pin4, LOW );

    pinMode ( m_pin1, OUTPUT);
    pinMode ( m_pin2, OUTPUT);
    pinMode ( m_pin3, OUTPUT);
    pinMode ( m_pin4, OUTPUT);
  }

  /////////////////////////////////////////////////////////////////////////////
  /// @brief Sets the final speed of the motor in RPM
  /// @TODO: the idea is that this method should be used to be able to accelerate
  ///        the motor. This is not implemented yet.
  /////////////////////////////////////////////////////////////////////////////
  void SetSpeed ( uint16 speed )
  {
    m_speed             = speed;
    m_delayBetweenSteps = 3;
  }

  /////////////////////////////////////////////////////////////////////////////
  /// @brief Run some steps
  /// @param steps The number of steps to run
  /////////////////////////////////////////////////////////////////////////////
  void Steps ( uint16 steps )
  {
    for (uint16 i = 0; i < steps; i++ )
    {
      if ( m_isHalfStep )
      {
        HalfStep();
      }
      else
      {
        FullStep();
      }
      if ( steps > 1 )
      {
        delay (m_delayBetweenSteps);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  /// @brief Set the motor direction
  /////////////////////////////////////////////////////////////////////////////
  void SetClockwise ( bool isClockwise )
  {
    m_isClockwise = isClockwise;
  }

  /////////////////////////////////////////////////////////////////////////////
  /// @brief Sets the motor to use Half Step.
  /////////////////////////////////////////////////////////////////////////////
  void SetHalfStep ( bool isHalfStep )
  {
    m_isHalfStep = isHalfStep;
  }

private:

  /////////////////////////////////////////////////////////////////////////////
  /// @brief Execute a full step in the motor.
  /////////////////////////////////////////////////////////////////////////////
  void FullStep ()
  {
    switch ( m_state )
    {
      case 0:
      {
        digitalWrite ( m_pin1, LOW  );
        digitalWrite ( m_pin2, LOW  );
        digitalWrite ( m_pin3, LOW  );
        digitalWrite ( m_pin4, HIGH );
      }
      break;
      case 1:
      {
        digitalWrite ( m_pin1, LOW  );
        digitalWrite ( m_pin2, LOW  );
        digitalWrite ( m_pin3, HIGH );
        digitalWrite ( m_pin4, LOW  );
      }
      break;
      case 2:
      {
        digitalWrite ( m_pin1, LOW  );
        digitalWrite ( m_pin2, HIGH );
        digitalWrite ( m_pin3, LOW  );
        digitalWrite ( m_pin4, LOW  );
      }
      break;
      case 3:
      {
        digitalWrite ( m_pin1, HIGH );
        digitalWrite ( m_pin2, LOW  );
        digitalWrite ( m_pin3, LOW  );
        digitalWrite ( m_pin4, LOW  );
      }
      break;
      default:
      {
        digitalWrite ( m_pin1, LOW  );
        digitalWrite ( m_pin2, LOW  );
        digitalWrite ( m_pin3, LOW  );
        digitalWrite ( m_pin4, LOW  );
      }
      break;
    }

    m_state = m_isClockwise ? m_state - 1 : m_state + 1;

    if ( m_state >= 4 )
    {
      m_state = 0;
    }
    if ( m_state < 0 )
    {
      m_state = 3;
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  /// @brief Executes a half step in the motor
  /////////////////////////////////////////////////////////////////////////////
  void HalfStep ()
  {
    switch ( m_state )
    {
      case 0:
      {
        digitalWrite ( m_pin1, LOW  );
        digitalWrite ( m_pin2, LOW  );
        digitalWrite ( m_pin3, LOW  );
        digitalWrite ( m_pin4, HIGH );
      }
      break;
      case 1:
      {
        digitalWrite ( m_pin1, LOW  );
        digitalWrite ( m_pin2, LOW  );
        digitalWrite ( m_pin3, HIGH );
        digitalWrite ( m_pin4, HIGH );
      }
      break;
      case 2:
      {
        digitalWrite ( m_pin1, LOW  );
        digitalWrite ( m_pin2, LOW  );
        digitalWrite ( m_pin3, HIGH );
        digitalWrite ( m_pin4, LOW  );
      }
      break;
      case 3:
      {
        digitalWrite ( m_pin1, LOW  );
        digitalWrite ( m_pin2, HIGH );
        digitalWrite ( m_pin3, HIGH );
        digitalWrite ( m_pin4, LOW  );
      }
      break;
      case 4:
      {
        digitalWrite ( m_pin1, LOW  );
        digitalWrite ( m_pin2, HIGH );
        digitalWrite ( m_pin3, LOW  );
        digitalWrite ( m_pin4, LOW  );
      }
      break;
      case 5:
      {
        digitalWrite ( m_pin1, HIGH );
        digitalWrite ( m_pin2, HIGH );
        digitalWrite ( m_pin3, LOW  );
        digitalWrite ( m_pin4, LOW  );
      }
      break;
      case 6:
      {
        digitalWrite ( m_pin1, HIGH );
        digitalWrite ( m_pin2, LOW  );
        digitalWrite ( m_pin3, LOW  );
        digitalWrite ( m_pin4, LOW  );
      }
      break;
      case 7:
      {
        digitalWrite ( m_pin1, HIGH );
        digitalWrite ( m_pin2, LOW  );
        digitalWrite ( m_pin3, LOW  );
        digitalWrite ( m_pin4, HIGH );
      }
      break;
      default:
      {
        digitalWrite ( m_pin1, LOW  );
        digitalWrite ( m_pin2, LOW  );
        digitalWrite ( m_pin3, LOW  );
        digitalWrite ( m_pin4, LOW  );
      }
      break;
    }
    m_state++;
    if ( m_state >= 8)
    {
      m_state = 0;
    }
  }

  uint16  m_numberOfStepsFullRotation;
  uint16  m_pin1;
  uint16  m_pin2;
  uint16  m_pin3;
  uint16  m_pin4;
  uint16  m_speed;
  uint32  m_delayBetweenSteps;
  int8    m_state;
  bool    m_isHalfStep;
  bool    m_isClockwise;
};

///////////////////////////////////////////////////////////////////////////////
/// @class InputGpio
/// @brief Represent an input gpio with debouncing options
///////////////////////////////////////////////////////////////////////////////
class InputGpio
{
public:

  /////////////////////////////////////////////////////////////////////////////
  /// @class Ctor
  /// @param pin The Arduino Pin
  /// @param isPullEnabled If the internal pull-up should be activated
  /// @param isDebounceEnalbed If should use debouncing algorithm
  /// @param debounceTime The debounce time in ms
  /////////////////////////////////////////////////////////////////////////////
  InputGpio ( uint8 pin, bool isInverted, bool isPullupEnabled, bool isDebounceEnalbed, uint16 debounceTime = 10 )
  : m_pin             ( pin )
  , m_isInverted      ( isInverted )
  , m_isPullupEnabled ( isPullupEnabled )
  , m_isDebounceEnabled (isDebounceEnalbed)
  , m_debounceTime    ( debounceTime )
  , m_value           ( false )
  , m_tempValue       ( false )
  , m_restartDebounce ( false )
  , m_isDeboucing     ( false )
  {
    pinMode ( pin, m_isPullupEnabled ? INPUT_PULLUP : INPUT );
  }

  /////////////////////////////////////////////////////////////////////////////
  /// @brief Process the debounce time counting
  /// @details This method counts the debouncing time, and it MUST be called
  ///          on a periodic interval according to the debounceTime resolution.
  ///          For instance, if the debounceTime is counted in ms, this method
  ///          should be called each 1ms.
  /////////////////////////////////////////////////////////////////////////////
  void ProcessDebounce ( uint16 timeInterval )
  {
    static uint16 timeCounter = 0;

    if ( true == m_isDebounceEnabled )
    {
      if ( m_isDeboucing && true == m_restartDebounce )
      {
        timeCounter = 0;
        m_restartDebounce = false;
      }
      else if ( m_isDeboucing )
      {
        timeCounter+= timeInterval;
        if ( timeCounter >= m_debounceTime )
        {
          m_isDeboucing = false;
          m_value       = m_tempValue;
        }
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  /// @brief Returns the value of the pin
  /// @details This method returns the value of the pin after applying the
  ///          debouncing algorithm, if configured.
  /////////////////////////////////////////////////////////////////////////////
  bool  read () 
  {
    bool  currentValue = digitalRead ( m_pin );

    if ( true == m_isDebounceEnabled )
    {
      // Already counting deboucing time, but detected a new value
      if ( m_isDeboucing && currentValue != m_tempValue )
      {
        m_restartDebounce = true;
      }
      // Not counting debouncing time and detected a new value
      else if ( false == m_isDeboucing && currentValue != m_value )
      {
        m_isDeboucing     = true;
        m_restartDebounce = true;
        m_tempValue       = currentValue;
      }
    }
    else
    {
      m_value = currentValue;
    }

    return m_value ^ m_isInverted;
  }

private:
  uint8   m_pin;
  bool    m_isInverted;
  bool    m_isPullupEnabled;
  bool    m_isDebounceEnabled;
  uint16  m_debounceTime;
  bool    m_value;
  bool    m_tempValue;
  bool    m_restartDebounce;
  bool    m_isDeboucing;
};

StepperMotor*   pStepper01  = NULL;
StepperMotor*   pStepper02  = NULL;
InputGpio*      pSw01       = NULL;
InputGpio*      pSw02       = NULL;
InputGpio*      pSw03       = NULL;
InputGpio*      pSw04       = NULL;

void Timer1Interrupt ()
{
  pSw01->ProcessDebounce( 1 );
  pSw02->ProcessDebounce( 1 );
  pSw03->ProcessDebounce( 1 );
  pSw04->ProcessDebounce( 1 );
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200, SERIAL_8N1);
  pStepper01 = new StepperMotor (2048, 6, 7, 8, 9);
  pStepper01->SetSpeed( 12 );
  pStepper02 = new StepperMotor (2048, 10, 11, 12, 13);
  pStepper02->SetSpeed( 12 );

  pSw01 = new InputGpio ( 2, true, true, true );
  pSw02 = new InputGpio ( 3, true, true, true );
  pSw03 = new InputGpio ( 4, true, true, true );
  pSw04 = new InputGpio ( 5, true, true, true );

  Timer1.initialize ( 1000 );
  Timer1.attachInterrupt ( Timer1Interrupt );

  interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:

  if ( true == pSw01->read() )
  {
    pStepper01->SetClockwise( false );
    pStepper01->Steps(32);
  }
  if ( true == pSw02->read() )
  {
    pStepper01->SetClockwise( true );
    pStepper01->Steps(32);
  }
  if ( true == pSw03->read() )
  {
    pStepper02->SetClockwise( false );
    pStepper02->Steps(32);
  }
  if ( true == pSw04->read() )
  {
    pStepper02->SetClockwise( true );
    pStepper02->Steps(32);
  }

}


