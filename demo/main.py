import os
import argparse

import wx
import wx.media
import wx.lib.buttons as buttons

from utils import load_dataset, GROUP_COMPOSITION_FILENAME, DEFAULT_HOP_LENGTH


bitmap_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'bitmaps')


class PlayerFrame(wx.Frame):
    def __init__(self, hop_lenght):
        wx.Frame.__init__(self, None, wx.ID_ANY, "Music synchronization player")

        self.panel = wx.Panel(self)

        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.on_timer)
        self.timer.Start(100)

        self.default_color = wx.ColourDatabase().Find('LIGHT GREY')
        self.hightlight_color = wx.ColourDatabase().Find("AQUAMARINE")

        sp = wx.StandardPaths.Get()
        self.hop_lenght = hop_lenght

        self.playback_sliders = []
        self.rec_medias = []
        self.rec_labels = []

        self.cur_dataset_dir = sp.GetDocumentsDir()
        self.composition_groups = {}
        self.cur_compose = None
        self.cur_alg = None
        self.cur_number_record = None

        self.set_menubar()
        self.set_panel()
        self.panel.GetSizer().Layout()

    def set_menubar(self):
        menubar = wx.MenuBar()

        file_menu = wx.Menu()
        load_dataset_item = file_menu.Append(wx.NewId(), "Load a dataset")
        menubar.Append(file_menu, 'File')
        self.SetMenuBar(menubar)
        self.Bind(wx.EVT_MENU, self.load_dataset, load_dataset_item)

    def set_panel(self):
        main_sizer = wx.BoxSizer(wx.VERTICAL)

        self.music_alg_sizer = self.set_compose_alg_panel()
        self.recs_sizer = self.set_recordings_panel()
        self.audio_bar_panel = self.set_audio_bar_panel()

        main_sizer.Add(self.music_alg_sizer, 0, wx.TOP|wx.BOTTOM|wx.CENTER, 10)
        main_sizer.Add(self.audio_bar_panel, 0, wx.BOTTOM|wx.CENTER, 10)
        main_sizer.Add(self.recs_sizer, 0, wx.BOTTOM|wx.EXPAND, 20)

        self.panel.SetSizer(main_sizer)

    def set_compose_alg_panel(self):
        music_alg_sizer = wx.BoxSizer(wx.HORIZONTAL)

        label_composition = wx.StaticText(self.panel, wx.ID_ANY, 'Composition:')
        self.composition_setup = wx.Choice(self.panel, choices=[])
        self.composition_setup.Bind(wx.EVT_CHOICE, self.on_set_composition)

        label_algorithm = wx.StaticText(self.panel, wx.ID_ANY, 'Algorithm:')
        self.alg_setup = wx.Choice(self.panel, choices=[])
        self.alg_setup.Bind(wx.EVT_CHOICE, self.on_set_algorithm)

        music_alg_sizer.Add(label_composition, 0, wx.ALL|wx.CENTER, border=5)
        music_alg_sizer.Add(self.composition_setup, 1, wx.ALL|wx.CENTER, border=5)
        music_alg_sizer.Add(label_algorithm, 0, wx.ALL|wx.CENTER, border=5)
        music_alg_sizer.Add(self.alg_setup, 1, wx.ALL|wx.CENTER, border=5)
        return music_alg_sizer

    def add_button(self, bitmap, handler, name, sizer):
        img = wx.Bitmap(os.path.join(bitmap_dir, bitmap))
        btn = wx.lib.buttons.GenBitmapButton(self.panel, bitmap=img, name=name)
        btn.SetInitialSize()
        btn.Bind(wx.EVT_BUTTON, handler)
        sizer.Add(btn, 0, wx.ALL, 1)
        btn.Enable(False)
        return btn

    def set_audio_bar_panel(self):
        audio_bar_sizer = wx.BoxSizer(wx.HORIZONTAL)

        self.prev_btn = self.add_button('player_prev.png', self.on_prev_recording, 'prev', audio_bar_sizer)

        img = wx.Bitmap(os.path.join(bitmap_dir, "player_play.png"))
        self.play_pause_btn = buttons.GenBitmapToggleButton(self.panel, bitmap=img, name="play")
        self.play_pause_btn.Enable(False)

        img = wx.Bitmap(os.path.join(bitmap_dir, "player_pause.png"))
        self.play_pause_btn.SetBitmapSelected(img)
        self.play_pause_btn.SetInitialSize()

        self.play_pause_btn.Bind(wx.EVT_BUTTON, self.on_play_recording)
        audio_bar_sizer.Add(self.play_pause_btn, 0, wx.ALL, 1)

        self.next_btn = self.add_button("player_next.png", self.on_next_recording, 'next', audio_bar_sizer)
        return audio_bar_sizer

    def set_recordings_panel(self):
        recs_sizer = wx.BoxSizer(wx.VERTICAL)
        return recs_sizer

    def add_recording(self, name, path):
        label = wx.StaticText(self.panel, wx.ID_ANY, '{} :'.format(name))
        media = wx.media.MediaCtrl(self.panel,
                                       # szBackend=wx.media.MEDIABACKEND_GSTREAMER,
                                       style=wx.SIMPLE_BORDER)

        media.Bind(wx.media.EVT_MEDIA_FINISHED, self.on_start_recording, media)
        media.Load(path)

        playback_slider = wx.Slider(self.panel, size=wx.DefaultSize)
        self.Bind(wx.EVT_SLIDER, self.on_seek, playback_slider)
        playback_slider.SetRange(0, media.Length())
        return label, media, playback_slider

    def set_recordings(self):
        for slider in self.playback_sliders:
            slider.Destroy()
        self.playback_sliders = []

        for media in self.rec_medias:
            media.Destroy()
        self.rec_medias = []

        for label in self.rec_labels:
            label.Destroy()
        self.rec_labels = []

        if self.recs_sizer.GetChildren():
            self.recs_sizer.Hide(0)
            self.recs_sizer.Remove(0)
        if self.cur_compose is not None:
            composition = self.composition_groups[self.cur_compose]
            for rec in composition.recordings:
                path = rec.path
                name = rec.name
                label, media, playback_slider = self.add_recording(name, path)
                playback_slider.SetBackgroundColour(self.default_color)
                self.rec_labels.append(label)
                self.rec_medias.append(media)
                self.playback_sliders.append(playback_slider)

                self.recs_sizer.Add(label, 0, wx.TOP|wx.LEFT|wx.ALIGN_LEFT, 30)
                self.recs_sizer.Add(playback_slider, 1, wx.LEFT|wx.RIGHT|wx.TOP|wx.EXPAND, 10)

            if len(composition.recordings) != 0:
                self.cur_number_record = 0
                self.playback_sliders[self.cur_number_record].SetBackgroundColour(self.hightlight_color)
            else:
                self.cur_number_record = None

            self.recs_sizer.Layout()
            self.panel.GetSizer().Layout()
            # self.Fit()

    def on_timer(self, event):
        if len(self.rec_medias) > 0:
            pass
            offset = self.rec_medias[self.cur_number_record].Tell()
            self.playback_sliders[self.cur_number_record].SetValue(offset)

            recs = self.composition_groups[self.cur_compose].recordings
            cur_rec = recs[self.cur_number_record]

            for numb_rec, rec in enumerate(recs):
                if numb_rec == self.cur_number_record:
                    continue

                rec_offset = cur_rec.get_map_value(
                    alg_name=self.cur_alg,
                    number_rec=rec.number,
                    hop_lenght=self.hop_lenght,
                    cur_time=offset
                )
                self.playback_sliders[numb_rec].SetValue(rec_offset)

    def on_seek(self, event):
        obj = event.GetEventObject()
        recs = self.composition_groups[self.cur_compose].recordings

        for i, playback_slider in enumerate(self.playback_sliders):
            if playback_slider == obj:
                main_offset = playback_slider.GetValue()
                main_rec = recs[i]

                for numb_rec, rec in enumerate(recs):
                    if numb_rec == i:
                        continue

                    rec_offset = main_rec.get_map_value(
                        alg_name=self.cur_alg,
                        number_rec=rec.number,
                        hop_lenght=self.hop_lenght,
                        cur_time=main_offset
                    )
                    self.playback_sliders[numb_rec].SetValue(rec_offset)

        offset = self.playback_sliders[self.cur_number_record].GetValue()
        self.rec_medias[self.cur_number_record].Seek(offset)

    def on_prev_recording(self, event):
        recordings = self.composition_groups[self.cur_compose].recordings
        next_number_record = (self.cur_number_record - 1) % len(recordings)
        self.turn_recording(next_number_record)

    def on_next_recording(self, event):
        recordings = self.composition_groups[self.cur_compose].recordings
        next_number_record = (self.cur_number_record + 1) % len(recordings)
        self.turn_recording(next_number_record)

    def turn_recording(self, next_number_record):
        recordings = self.composition_groups[self.cur_compose].recordings

        next_rec = recordings[next_number_record]
        cur_offset = self.rec_medias[self.cur_number_record].Tell()
        next_offset = recordings[self.cur_number_record].get_map_value(
            alg_name=self.cur_alg,
            number_rec=next_rec.number,
            hop_lenght=self.hop_lenght,
            cur_time=cur_offset
        )
        self.rec_medias[next_number_record].Seek(next_offset)
        self.rec_medias[self.cur_number_record].Pause()
        if self.play_pause_btn.GetToggle():
            self.rec_medias[next_number_record].Play()

        self.playback_sliders[self.cur_number_record].SetBackgroundColour(self.default_color)
        self.playback_sliders[next_number_record].SetBackgroundColour(self.hightlight_color)

        self.cur_number_record = next_number_record

    def on_play_recording(self, event):
        if not event.GetIsDown():
            self.on_pause_recording()
            return
        if self.cur_number_record is not None:
            ok = self.rec_medias[self.cur_number_record].Play()
            if not ok:
                wx.MessageBox(self.panel,
                              "Unable to Play media : Unsupported format?",
                              "ERROR",
                              wx.ICON_ERROR | wx.OK)
        event.Skip()

    def on_start_recording(self, event):
        pass

    def on_pause_recording(self):
        self.rec_medias[self.cur_number_record].Pause()

    def on_set_composition(self, event):
        self.cur_compose = self.composition_setup.GetString(self.composition_setup.GetSelection())
        dtw_maps = self.composition_groups[self.cur_compose].recordings[0].dtw_maps

        self.alg_setup.Clear()
        for alg in dtw_maps.keys():
            self.alg_setup.Append(alg)
        self.set_recordings()

        if len(dtw_maps.keys()) > 0:
            self.alg_setup.SetSelection(n=0)
        else:
            self.alg_setup = None
        self.on_set_algorithm(None)

    def on_set_algorithm(self, event):
        n_sel = self.alg_setup.GetSelection()
        if n_sel == wx.NOT_FOUND:
            self.prev_btn.Enable(False)
            self.play_pause_btn.Enable(False)
            self.next_btn.Enable(False)
        else:
            self.cur_alg = self.alg_setup.GetString(n_sel)
            self.prev_btn.Enable(True)
            self.play_pause_btn.Enable(True)
            self.next_btn.Enable(True)

    def load_dataset(self, event):
        dlg = wx.DirDialog(
            self, message="Choose a dataset dir(mush have '{}' file)".format(GROUP_COMPOSITION_FILENAME),
            defaultPath=self.cur_dataset_dir,
        )
        if dlg.ShowModal() == wx.ID_OK:
            self.cur_dataset_dir = dlg.GetPath()
            try:
                self.composition_groups = load_dataset(self.cur_dataset_dir)
                self.composition_setup.Clear()
                for compose in self.composition_groups.keys():
                    self.composition_setup.Append(compose)
                if len(self.composition_groups) > 0:
                    self.composition_setup.SetSelection(n=0)
                    self.on_set_composition(None)
            except Exception as err:
                msg = wx.MessageDialog(self.panel,
                                       message=str(err),
                                       style=wx.ICON_ERROR | wx.OK)
                msg.ShowModal()
                msg.Destroy()
        dlg.Destroy()


if __name__ == "__main__":
    parser = argparse.ArgumentParser('')

    parser.add_argument('--hop_lenght', type=int, default=DEFAULT_HOP_LENGTH,
                        help='Hop length(ms), default: {} ms'.format(DEFAULT_HOP_LENGTH))
    args = parser.parse_args()

    app = wx.App(False)
    frame = PlayerFrame(args.hop_lenght)
    frame.Show()
    app.MainLoop()