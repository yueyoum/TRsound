#!/usr/bin/env python

import urllib
import urllib2
import re
import sys
import os
import tempfile
import ctypes


# REPLACE THIS WITH YOUR PATH
WSOUND = os.path.dirname(os.path.realpath(__file__)) + '/libwsound.so'
print WSOUND


class GoogleTrans(object):
    def __init__(self):
        self.URL = 'http://translate.google.cn/translate_a/t?'
        self.TTS_URL = 'http://translate.google.cn/translate_tts?'
        self.EN_PATTERN = re.compile('[a-zA-Z]+')
        self.RES_PATTERN = re.compile('\[.+?\]')
        
        if not os.path.exists(WSOUND):
            raise Exception("%s does not exist, replace with your path" % WSOUND)
        
        self.libwsound = ctypes.cdll.LoadLibrary( WSOUND )
        
        self.last_word = ''
        self.tempfile = tempfile.mktemp(prefix='.tmpWsound')
        
        self.opener = urllib2.build_opener(urllib2.HTTPCookieProcessor())
        self.opener.addheaders = [
                ('User-Agent', 'Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:14.0) Gecko/20100101 Firefox/14.0.1'),
            ]
        
        
        
    def sound(self):
        if not os.path.exists(self.tempfile):
            print "tempfile does not exist"
            return
        
        self.libwsound.decode(self.tempfile);
        
        
    def get_tts(self):
        if not self.last_word:
            return
        
        _param = {
            'ie': 'UTF-8',
            'q': self.last_word,
            'total': 1,
            'idx': 0,
            'textlen': len(self.last_word),
            'prev': 'input',
            'tl': 'zh-CN'
        }
        
        if self.EN_PATTERN.search(self.last_word) is not None:
            _param['tl'] = 'en'
            
        url = '%s%s' % (self.TTS_URL, urllib.urlencode(_param))
        response = self.opener.open(url)
        
        with open(self.tempfile, 'w') as f:
            f.write(response.read())
        
        self.sound();
        
        
    
    def _request(self, text):
        _param = {
            'client': 't',
            'text': text,
            'ie': 'UTF-8',
            'oe': 'UTF-8',
            'mulitres': 1,
            'otf': 1,
            'ssel': 0,
            'tsel': 0,
            'sc': 1
        }
        
        if self.EN_PATTERN.search(text) is not None:
            # englist to chinese
            _param['sl'] = 'en'
            _param['tl'] = 'zh-CN'
            _param['hl'] = 'zh-CN'
        else:
            _param['sl'] = 'zh-CN'
            _param['tl'] = 'en'
            _param['hl'] = 'en'
            
        url = '%s%s' % (self.URL, urllib.urlencode(_param))
        response = self.opener.open(url);
        return response.read()
        
        
    def trans(self, text):
        self.last_word = text;
        
        _res = self.RES_PATTERN.findall( self._request(text) )
        _res = _res[:2]
        return [r.replace('[', '').replace(']', '') for r in _res]


if __name__ == '__main__':
    googletrans = GoogleTrans()
    
    commands = {
        'exit': sys.exit,
        's': googletrans.get_tts,
    }
    
        
    while True:
        text = raw_input('> ')
        if text.lower() in commands:
            commands[text.lower()]()
            continue
            
        for line in googletrans.trans(text):
            print line
        
    sys.exit(0)
