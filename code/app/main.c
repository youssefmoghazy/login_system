//MCAL Layer
#include"../MCAL/DIO/DIO_interface.h"
#include"../MCAL/EXTI/EXTI_Interface.h"
#include"../MCAL/GI/GI_Interface.h"
//LIB Layer
#include"../lib/STD_TYPES/STD_TYPES.h"
//HAL Layer
#include"../HAL/LCD/LCD_Interface.h"
#include"../HAL/KPD/KPD_interface.h"
#include"../HAL/DC_Motor/DC_Motor_Interface.h"
#include"../HAL/Stepper_Motor/Stepper_Motor_Interface.h"
#include<util/delay.h>
#define F_CPU 8000000UL

// Define the Password for the login system
#define PasswordLength      4
#define Password            {'1','2','3','4'}

// Define delay macros
#define SHORT_DELAY_MS      200
#define LONG_DELAY_MS       500

// Define options
#define DC_MOTOR_OPTION_CW       '1'
#define DC_MOTOR_OPTION_CCW      '2'
#define STEPPER_MOTOR_OPTION_CW  '1'
#define STEPPER_MOTOR_OPTION_CCW '2'
#define EXIT_OPTION              '3'

// Function prototypes
void displayOptionsPage(void);
void takeUserOption(u8 *option);
u16 stepper_angle(u8 * array);

// Function for the login system
void loginSystem(void){

    // Initialise LCD and motors
	DIO_voidInit();
    LCD_voidInit();
    stepperMotor_VoidInti();
    DC_motor_u8Inti(DC_MOTOR1);

    // Define the saved password and the entered password
    u8 savedPass[] = Password ;
    u8 pass[PasswordLength];
    u8 Index = u8_intial_value, checkCounter = u8_intial_value;
    u8 homeFlag = u8_intial_value, inputFlag = u8_intial_value;

    // Display welcome message
    LCD_voidSendString("Welcome..");
    _delay_ms(LONG_DELAY_MS);
    LCD_voidClear();
    LCD_u8GoToXY(LCD_u8_Line1,0);
    LCD_voidSendString("Enter password");
    _delay_ms(SHORT_DELAY_MS);
    LCD_u8GoToXY(LCD_u8_Line2,5);

    // Infinite loop for the login system
    while(1){
        u8 key = u8_intial_value, order = u8_intial_value;

        // Get the key state from the keypad
        KPD_u8GetKeyState(&key);

        // If a key is pressed, display '*' and store the entered key in 'pass' array
        if(key != KPD_u8_KEY_NOT_PRESSED){
            LCD_voidSendChar('*');
            pass[Index] = key;
            Index++;
            checkCounter++;
        }

        // If the entered password is complete
        if(checkCounter == PasswordLength){
            // Compare the entered password with the saved password
            for(int j = 0 ; j < PasswordLength ; j++){
                if(pass[j] != savedPass[j]){
                    inputFlag = 1;
                    break;
                }
            }

            // If the passwords match, display the options menu
            if (!inputFlag){
                // Display welcome message and options
                if(!homeFlag){
                    LCD_voidClear();
                    LCD_voidSendString("Welcome...");
                    LCD_u8GoToXY(1, 0);
                    LCD_voidSendString("Choose Option...");
                    homeFlag = 1;
                    _delay_ms(LONG_DELAY_MS);
                }
                // Display options
                displayOptionsPage();
                // Get the user's selected option
                takeUserOption(&order);

                // Handle the selected option
                if(order == DC_MOTOR_OPTION_CW){
                    // Display rotation options for DC Motor
                    displayRotationOptions();
                    // Get the user's selected rotation option
                    takeUserOption(&order);
                    // Rotate the DC Motor based on the selected option
                    if(order == DC_MOTOR_OPTION_CW){
                        DC_motor_u8Move(DC_MOTOR1, DC_MOTOR_ROTATE_CW);
                    } else if (order == DC_MOTOR_OPTION_CCW){
                        DC_motor_u8Move(DC_MOTOR1, DC_MOTOR_ROTATE_CCW);
                    }
                } else if(order == '2'){
                    // Take Stepper Motor Angle
                    LCD_voidClear();
                    LCD_voidSendString("Enter Angle:");
                    u8 stepperMotorAngle[3];
                    u8 angleCounter = 0 ;
                    // Display and store the angle entered by the user
                    while(angleCounter != 3){
                        KPD_u8GetKeyState(&key);
                        if(key != KPD_u8_KEY_NOT_PRESSED && key != '='){
                            LCD_voidSendChar(key);
                            stepperMotorAngle[angleCounter] = key;
                            angleCounter++;
                            _delay_ms(SHORT_DELAY_MS);
                        } else if(key == '=') {
                            // if the user entered an angle of one or two digits
                            if(angleCounter == 1) {
                                stepperMotorAngle[1] = 'a';
                                break;
                            }
                            if(angleCounter == 2) {
                                stepperMotorAngle[2] = 'a';
                                break;
                            }
                        }
                    }
                    // Display rotation options for Stepper Motor
                    displayRotationOptions();
                    // Get the user's selected rotation option
                    takeUserOption(&order);
                    // Rotate the Stepper Motor based on the entered angle and the selected option
                    if(order == STEPPER_MOTOR_OPTION_CW){
                        stepperMotor_u8RotateBy(stepper_angle(stepperMotorAngle), ROTATE_CW);
                        _delay_ms(LONG_DELAY_MS);
                    } else if (order == STEPPER_MOTOR_OPTION_CCW){
                        stepperMotor_u8RotateBy(stepper_angle(stepperMotorAngle), ROTATE_CCW);
                        _delay_ms(LONG_DELAY_MS);
                    }
                } else if (order == EXIT_OPTION){
                    // Exit Section: Display exit message and stop all motors
                    LCD_voidClear();
                    LCD_voidSendString("Exiting .. ");
                    _delay_ms(LONG_DELAY_MS);
                    LCD_voidClear();
                    /*Stop All Motors*/
                    DC_motor_u8Stop(DC_MOTOR1);
                    stepperMotor_u8RotateBy(0,ROTATE_CW);
                    break;
                }
            } else {
                // Entered password is wrong, display error message and allow the user to try again
                Index = u8_intial_value;
                checkCounter = u8_intial_value;
                inputFlag = u8_intial_value;
                LCD_voidClear();
                LCD_voidSendString("Wrong Password");
                LCD_u8GoToXY(1, 0);
                LCD_voidSendString("Try Again!");
                _delay_ms(LONG_DELAY_MS);
                LCD_voidClear();
                LCD_voidSendString("Enter Password:");
                LCD_u8GoToXY(1, 0);
            }
        }
    }
}

// Main function
int main(){

    // Enable Global Interrupts
    GI_voidEnable();

    // Enable External Interrupt 0 with rising edge trigger
    EXTI_u8EXTIEnable(EXTI_u8_INTI0, EXTI_u8_RAISING_ADGE);

    // Set the callback function for External Interrupt 0 to the loginSystem function
    EXTI_u8EXTISetCallBack(EXTI_u8_INTI0, &loginSystem);

    // Infinite loop
    while(1);

    return 0;
}

// Function to display the options page on the LCD
void displayOptionsPage()
{
    LCD_voidClear();
    LCD_voidSendString("1-DCMotor 3-Exit");
    LCD_u8GoToXY(LCD_u8_Line2, 0);
    LCD_voidSendString("2-Stepper Motor");
}

// Function to take the user's option from the keypad
void takeUserOption(u8 *option)
{
    while(1)
    {
        KPD_u8GetKeyState(option);
        if(*option == DC_MOTOR_OPTION_CW || *option == DC_MOTOR_OPTION_CCW
        	|| *option == STEPPER_MOTOR_OPTION_CW || *option == STEPPER_MOTOR_OPTION_CCW || *option == EXIT_OPTION)
        {
        	break;
        }
    }
}

// Function to display the rotation options for the motors
void displayRotationOptions(){
    LCD_voidClear();
    LCD_voidSendString("1-Rotate CW ");
    LCD_u8GoToXY(LCD_u8_Line2, 0);
    LCD_voidSendString("2-Rotate CCW");
}

u16 stepper_angle(u8 * array){
    u16 k = u16_intial_value;
    for(u8 i = 0 ; i < 3 ; i++){
        // Ignore non-entered digits
        if(array[i] == 'a') break;
        k = k * 10 + (array[i] - '0');
    }
    return k;
}

