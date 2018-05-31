# -*- coding: utf-8 -*-

import wx, wx.media


class TestPanel(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None)
        self.testMedia = wx.media.MediaCtrl(self,
                                            szBackend=wx.media.MEDIABACKEND_GSTREAMER,
                                            style=wx.SIMPLE_BORDER)

        self.MOVE = ('1.wav', '1.wav', '1.wav')
        self.Mrange = 0
        self.testMedia.Bind(wx.media.EVT_MEDIA_LOADED, self.play, self.testMedia)
        self.testMedia.Bind(wx.media.EVT_MEDIA_FINISHED, self.play2, self.testMedia)
        self.testMedia.Load(self.MOVE[self.Mrange])

    def play(self, e):
        print("Playing:",self.MOVE[self.Mrange])
        self.testMedia.Play()

    def play2(self, e):
        self.testMedia.Stop()
        self.Mrange += 1
        print(self.Mrange)
        if self.Mrange > len(self.MOVE) -1:
            self.Mrange = 0
        self.testMedia.Load(self.MOVE[self.Mrange])


if __name__ == '__main__':
    app = wx.App()
    Fream = TestPanel()
    Fream.Show(True)
    app.MainLoop()