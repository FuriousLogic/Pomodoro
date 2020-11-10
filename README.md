# Pomodoro

## Arduino project for simple Pomodoro helper

### States
This will control 4 leds, 1 red, 1 yellow & 2 greens.  Plus a piezo buzzer.
The stares are:
- Neutral (yellow)
- Live pomodoro (red)
- Pomodoro completed (red & yellow)
- Short break (green)
- Long break (green & green)
- Break over (green & yellow)

### Flow
Neutral > button > Live pomodoro
Live pomodoro > timer > Pomodoro completed
Pomodoro completed > button > Short/Long break (3 short - 1 Long)
Short/Long break > timer > Break over
Break over > button > Neutral