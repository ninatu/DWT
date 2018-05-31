import os
import json

import wx
import wx.media
import wx.lib.buttons as buttons

from utils import load_dataset, GROUP_COMPOSITION_FILENAME

bitmap_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'bitmaps')


class PlayerFrame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, wx.ID_ANY, "Music synchronization player")

        wx.Panel(self)

        sp = wx.StandardPaths.Get()
        self.cur_dataset_dir = sp.GetDocumentsDir()
        self.cur_compose = None
        self.cur_alg = None

        self.set_menubar()
        self.set_panel()

    def set_menubar(self):
        menubar = wx.MenuBar()

        file_menu = wx.Menu()
        load_dataset_item = file_menu.Append(wx.NewId(), "Load a dataset")
        menubar.Append(file_menu, 'File')
        self.SetMenuBar(menubar)
        self.Bind(wx.EVT_MENU, self.load_dataset, load_dataset_item)

    def set_panel(self):
        main_sizer = wx.BoxSizer(wx.VERTICAL)

        self.music_alg_sizer = self.set_music_alg_panel()
        self.recs_sizer = self.set_recordings_panel()
        self.audio_bar_panel = self.set_audio_bar_panel()
        main_sizer.Add(self.music_alg_sizer, flag=wx.EXPAND)
        main_sizer.Add(self.recs_sizer, flag=wx.EXPAND)
        main_sizer.Add(self.audio_bar_panel, flag=wx.BOTTOM)

        self.SetSizer(main_sizer)

    def set_music_alg_panel(self):
        music_alg_sizer = wx.BoxSizer(wx.HORIZONTAL)

        self.music_setup = wx.ComboBox(self, choices=[])
        music_alg_sizer.Add(self.music_setup, proportion=1, border=30)
        self.music_setup.Bind(wx.EVT_COMBOBOX, self.set_composition)

        self.alg_setup = wx.ComboBox(self, choices=[])
        music_alg_sizer.Add(self.alg_setup, proportion=1, border=30)
        return music_alg_sizer

    def set_audio_bar_panel(self):
        audio_bar_sizer = wx.BoxSizer(wx.HORIZONTAL)

        self.add_button('player_prev.png', self.turn_prev_recording, 'prev', audio_bar_sizer)

        img = wx.Bitmap(os.path.join(bitmap_dir, "player_play.png"))
        self.playPauseBtn = buttons.GenBitmapToggleButton(self, bitmap=img, name="play")
        self.playPauseBtn.Enable(False)

        img = wx.Bitmap(os.path.join(bitmap_dir, "player_pause.png"))
        self.playPauseBtn.SetBitmapSelected(img)
        self.playPauseBtn.SetInitialSize()

        self.playPauseBtn.Bind(wx.EVT_BUTTON, self.play_recording)
        audio_bar_sizer.Add(self.playPauseBtn, 0, wx.LEFT, 3)

        # self.add_button("player_stop.png", self.stop_recording, 'stop', audioBarSizer)
        self.add_button("player_next.png", self.turn_next_recording, 'next', audio_bar_sizer)

        return audio_bar_sizer

    def set_recordings_panel(self):
        recs_sizer = wx.BoxSizer(wx.VERTICAL)
        return recs_sizer

    def add_button(self, bitmap, handler, name, sizer):
        img = wx.Bitmap(os.path.join(bitmap_dir, bitmap))
        btn = wx.lib.buttons.GenBitmapButton(self, bitmap=img, name=name)
        btn.SetInitialSize()
        btn.Bind(wx.EVT_BUTTON, handler)
        sizer.Add(btn, 0, wx.LEFT, 3)

    def turn_prev_recording(self):
        pass

    def turn_next_recording(self):
        pass

    def play_recording(self):
        pass

    def set_composition(self, event):
        self.cur_compose = event.GetValue()

        dtw_maps = self.group_composition[self.cur_compose].recordings[0].dtw_maps

        self.music_setup.Clear()
        for alg in dtw_maps.keys():
            self.music_setup.Append(alg)

    def load_dataset(self, event):
        dlg = wx.DirDialog(
            self, message="Choose a dataset dir(mush have '{}' file)".format(GROUP_COMPOSITION_FILENAME),
            defaultPath=self.cur_dataset_dir,
        )
        if dlg.ShowModal() == wx.ID_OK:
            self.cur_dataset_dir = dlg.GetPath()
            try :
                self.group_composition = load_dataset(self.cur_dataset_dir)
            except Exception as err:
                msg = wx.MessageDialog(self,
                                       message=str(err),
                                       style=wx.ICON_ERROR | wx.OK)
                msg.ShowModal()
                msg.Destroy()
            self.music_setup.Clear()
            for compose in self.group_composition.keys():
                self.music_setup.Append(compose)
        dlg.Destroy()


if __name__ == "__main__":
    app = wx.App(False)
    frame = PlayerFrame()
    frame.Show()
    app.MainLoop()