import tkinter as tk
from tkinter.font import *
import serial as ser

class Button:
    #initializations

    # constructor 
    def __init__(self, p_root, p_text, p_value, p_x, p_y, var):
        self.root = p_root
        self.text = p_text
        self.val = p_value
        self.x = p_x
        self.y = p_y
        self.var = var

    # methods
    def createButton(self, p_font):
        button = tk.Radiobutton(self.root, text=self.text, value=self.val, variable=self.var)
        button.place(x=self.x, y=self.y)
        button.configure(font=p_font)

        
    def createExit(self, p_font):
        exit = tk.Button(self.root, text=self.text, width=25, command=self.root.destroy, bg="red")
        exit.place(x=self.x, y=self.y)
        exit.configure(font=p_font)
    
class Heading:
    # class attributes
    
    def __init__(self, p_root, p_text, p_font):
        self.root = p_root
        self.text = p_text
        self.font = p_font
    

    def createHeading(self):
        heading = tk.Label(self.root, text=self.text)
        heading.pack()
        heading.configure(font=self.font)

    def createSubHeading(self, p_x, p_y):
        subHeading = tk.Label(self.root, text=self.text)
        subHeading.place(x=p_x, y=p_y)
        subHeading.configure(font=self.font)

def createWindow(p_chipX, p_chipY, p_inputX, p_inputY):
    # create the root
    m_root = tk.Tk()
    m_root.title("IC Tester V1")
    m_root.geometry("800x600")
    
    # Create Fonts
    m_headingFont = Font(family="Astral", size=25, weight="bold")
    m_subHeadFont = Font(family="Astral", size=17, weight="bold")
    
    # create a heading
    m_heading = Heading(m_root, "Select Your Chip Below", m_headingFont)
    m_heading.createHeading()

    # Chip Subheading
    m_chipSelect = Heading(m_root, "Gate Type", m_subHeadFont)
    m_chipSelect.createSubHeading(p_chipX, p_chipY)

    # Input Subheading
    m_inputSelect = Heading(m_root, "Number of Inputs", m_subHeadFont)
    m_inputSelect.createSubHeading(p_inputX, p_inputY)

    return m_root

def main():
    # starting values of the chip and input columns
    chipX = 125
    chipY = 70
    inputX = 100
    inputY = 300

    # Create the root window
    root = createWindow(chipX, chipY, inputX, inputY)
    chipY += 40
    inputY += 40

    # create font for individual options
    optionFont = Font(family="Astral", weight="bold")
    exit = Button(root, "Exit", 0, 20, 530, 0)
    exit.createExit(optionFont)

    # gates and inputs
    currGate = tk.IntVar(value=0)   # shared IntVar for all options
    currInput = tk.IntVar(value=0)

    # Individual buttons
    singleInput = tk.Radiobutton(root, text="One Input", variable=currInput, value=1, font=optionFont)
    doubleInput = tk.Radiobutton(root, text="Two Inputs", variable=currInput, value=2, font=optionFont)
    ternaryInput = tk.Radiobutton(root, text="Three Inputs", variable=currInput, value=3, font=optionFont)
    quadInput = tk.Radiobutton(root, text="Four Inputs", variable=currInput, value=4, font=optionFont)
    inputButtons = [singleInput, doubleInput, ternaryInput, quadInput]
    
    # trace the gate
    def gateTracer(*args):
        for input in inputButtons:
            input.place_forget() # hide
        if(currGate.get() == 1):    # NOT gate
            inputButtons[0].place(x=inputX, y=inputY)
        elif(currGate.get() in [2, 3, 4, 5, 6]):
            currButton = 0
            newY = inputY
            while currButton < 3:
                inputButtons[currButton + 1].place(x=inputX, y=newY)
                newY += 30
                currButton += 1


    # Begin creating buttons
    gates = ["NOT Gate", "AND Gate", "OR Gate", "NAND Gate", "NOR Gate", "XOR Gate"]    # all possible gates
    currGate.trace_add("write", gateTracer)

    # Create chip and input buttons
    for i, gate in enumerate(gates):
        button = Button(root, gate, i + 1, chipX, chipY, currGate)
        button.createButton(optionFont)
        chipY += 30
    
    root.mainloop()
    print(button.var.get())
    return 0
if __name__ == '__main__':
    main()
