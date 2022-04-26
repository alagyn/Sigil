import unittest
import log
import os

class SetupTest(unittest.TestCase):
    def test_setup(self):
        ntl = os.getenv('TEST_LOG')
        if ntl is None or ntl == '0':
            log.enableLogging(False)
