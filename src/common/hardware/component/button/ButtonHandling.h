typedef enum eButtonMode_t
{
  IDLE_BUTTON,
  RISING_BUTTON,
  FALLING_BUTTON
} eButtonMode_t;

void ButtonHandling_init(void);

void ButtonHandling_executeTask(eButtonMode_t eEvent);
