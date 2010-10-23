#include <reg52.h>
#include "keyboard_driver.h"


/* Global variable declaration */
bit bOneClick;
bit bLongPress;
bit bDoubleClick;
bit bIsInDoubleClick;
bit bAllowLongPress;

byte ucBBtn;           /* possible clicked button before */
byte ucCBtn;           /* current possible clicked button */
byte ucLastPress;      /* last certain clicked button */
byte ucHighTime;
byte ucLowTime;
byte ucHighTimeBefore;

int CheckState();
void JudgeNoChange();
void JudgeChange();


/***************************************************************
Function:JudgeDec()
Discription:Every time we get into the function,
            we check the keyboard status
***************************************************************/
void JudgeDec()
{
    CheckState();
    if(ucBBtn == ucCBtn)
    {
        JudgeNoChange();
    }
    else
    {
        JudgeChange();
    }
    return;
}
/***************************************************************
Function:JudgeDec()
Discription:Every time we find that the former key and the 
            current key are the same, we get in this function.
***************************************************************/
void JudgeNoChange()
{
    //relax
    if(ucCBtn == KEY_NULL)
    {        
        if(ucHighTime == 0)
        {            
            if(ucLowTime == 0)
            {
                ucHighTimeBefore = ucHighTime;
                return;
            }
            if(bIsInDoubleClick)
            {
                ucHighTimeBefore = ucHighTime;
                return;
            }
            if(ucHighTimeBefore == 1)
            {
                bOneClick = 1;
                ucHighTimeBefore = ucHighTime;
            }
        }
        else
        {
            ucHighTimeBefore = ucHighTime;
            ucHighTime--;
        }
    }
    //hold
    else
    {
        if(ucLowTime == 0)
        {
            bLongPress = 1;
        }
        else
        {
            ucLowTime--;            
        }
    }    
}


/***************************************************************
Function:JudgeDec()
Discription:Every time we find that the former key and the 
            current key are the same, we get in this function.
***************************************************************/
void JudgeChange()
{
    bit bTempInDoubleClick;
    if(ucCBtn == KEY_NULL)
    {
        ucLastPress = ucBBtn;
        ucHighTime = 10;
    }
    else
    {
        bTempInDoubleClick = bIsInDoubleClick;
        bIsInDoubleClick = 0;
        bAllowLongPress = 1;
        if(ucLowTime == 0)
        {
            ucLowTime = 100;
            return;
        }
        else
        {
             ucLowTime = 100;
        }
        if(ucHighTime == 0)
        {
            return;
        }
        if(ucCBtn == ucLastPress)
        {
            if(bTempInDoubleClick == 1)
            {
                return;
            }
            bIsInDoubleClick = 1;
            bDoubleClick = 1;
        }
        else
        {
            bOneClick = 1;
        }
    }

    return;
}

/***************************************************************
Function:   JudgeDec()
Discription:This function is used to check the button pressed 
Output:     The number of the pressed button ( 1 - 7 )
            return 0 means nothing pressed
***************************************************************/
int CheckState()
{
    /* set the button pressed before */
    ucBBtn = ucCBtn;

    switch (P0 | 0x7F)  /* get the low 7 bits of P0 */
    {
        case 0xFD:  /* S1 */
            ucCBtn = KEY_LEFT;
            break;
        case 0xF7:  /* S2 */
            ucCBtn = KEY_OK;
            break;
        case 0xEF:  /* S3 */ 
            ucCBtn = KEY_RIRHT;
            break;        
        case 0xFE:  /* S4 */
            ucCBtn = KEY_YES;
            break;
        case 0xDF:  /* S5 */
            ucCBtn = KEY_DOWN;
            break;
        case 0xFB:  /* S6 */
            ucCBtn = KEY_UP;
            break;
        case 0xBF:  /* S7 */
            ucCBtn = KEY_NO;
            break;
        default:  /* no button clicked */
            break;
    }

    ucCBtn = KEY_NULL;
    return 0;
}


/***************************************************************
Function:InitKeyState()
Discription:This function is used to initialize the vars used 
            in getting the status of the keyboard
***************************************************************/
void InitKeyState()
{
    ucBBtn = 0;
    ucCBtn = 0;
    ucLastPress = 0;  
    ucHighTime = 0;
    ucLowTime = 0;

    bOneClick = 0;
    bLongPress = 0;
    bDoubleClick = 0;
    bIsInDoubleClick = 0;
    bAllowLongPress = 0;
}

/***************************************************************
Function:GetKeyState()
Discription:The fuction is the main interface of the lib
***************************************************************/
SKeyEvent GetKeyState()
{
    SKeyEvent returnValue;
    if(bOneClick == 1)
    {
        bOneClick = 0;
        returnValue.eEventKind = SingleClick;        
        returnValue.ucKeyNum = ucLastPress;
    }
    else if(bDoubleClick == 1)
    {
        bDoubleClick = 0;
        returnValue.eEventKind = DoubleClick;
        returnValue.ucKeyNum = ucLastPress;
    }
    else if(bLongPress == 1)
    {        
        bLongPress = 0;
        if(!bAllowLongPress)
        {
            returnValue.eEventKind = Nothing;
            return returnValue;
        }
        returnValue.eEventKind = LongPress;  
        returnValue.ucKeyNum = ucCBtn;
        bAllowLongPress = 0;
    }
    else
    {
        returnValue.eEventKind = Nothing;
        returnValue.ucKeyNum = 0xff;
    }

    //if (returnValue.ucKeyNum == 10)     // make key 10 to key 0
    //    returnValue.ucKeyNum= 0;

    return returnValue;
}

