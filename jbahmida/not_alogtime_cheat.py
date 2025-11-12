import pyautogui
import random
import time

# Get screen size
screen_width, screen_height = pyautogui.size()

try:
    while True:
        # Generate random coordinates
        x = random.randint(0, screen_width - 1)
        y = random.randint(0, screen_height - 1)
        
        # Move cursor to random position
        pyautogui.moveTo(x, y, duration=0.5)
        
        print(f"Moved cursor to: ({x}, {y})")
        
        # Wait 30 seconds
        time.sleep(30)
        
except KeyboardInterrupt:
    print("\nScript stopped by user")
