""""
This is a file to perform unittests on the function in the Dynamic best fit python file to make sure each
of the functions return something similar to what is required
"""

from BestfitDynamic import *
import unittest


class TestBestFitDynamicClass(unittest.TestCase):

    def test_random_size(self):
        """
        Tests the size function
        """
        x = random_size()
        self.assertTrue(isinstance(x, int))
        self.assertTrue(0 < x <= 100)

    def test_in_or_out(self):
        """
        Tests the in or out func
        """
        x = in_or_out()
        self.assertTrue(x == 'in' or x == 'out')

    def test_random_job(self):
        """
        Tests whether the string is returned properly
        """
        x = random_job_number()
        self.assertTrue('job' in x)
        y = x.split('job')[1]
        y = int(y)
        self.assertTrue(0 < y <= 10)

    def test_print_memory(self):
        """
        Test the  print memory function
        """
        #         0     1     2     3     4          5         6           7        8       9       10      11
        memory = ['OS', 'OS', 'OS', None, None, 'cookiecut', 'cookiecut', 'job1', 'job1', 'job1', 'job2', 'job3',
                  'job3', 'cookie']

        from_func = print_memory(memory)
        expected = ['OS', None, 'cookiecut', 'job1', 'job2', 'job3', 'cookie']

        self.assertTrue(from_func == expected)


if __name__ == '__main__':
    unittest.main()
