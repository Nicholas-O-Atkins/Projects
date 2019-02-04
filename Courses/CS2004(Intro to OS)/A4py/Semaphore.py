"""
author: nicholas Atkins Due date: 20th Mar 2018
This is an object that hold an int value and is used as a signal in the main Holder.py program
This is not the place where you run the program, information is in the Holder.py comments at the top
"""

class Semaphore:
    """
    This is an object that given a int value will be able to be modified
    """
    def __init__(self, val):
        """
        given an integer it will initialize the semaphore

        :param val: integer value for the semaphore
        """
        self.value = val
