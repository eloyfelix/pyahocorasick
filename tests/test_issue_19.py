# -*- coding: utf-8 -*-
"""
    Aho-Corasick string search algorithm.

    Author    : Wojciech Muła, wojciech_mula@poczta.onet.pl
    WWW       : http://0x80.pl
    License   : public domain
"""

import ahocorasick


def test_issue_19():
    A = ahocorasick.Automaton()
    for index, word in enumerate("he her hers she".split()):
        A.add_word(word, (index, word))
        A.clear()
