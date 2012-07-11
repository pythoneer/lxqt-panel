/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Razor - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012 Razor team
 * Authors:
 *   Johannes Zellner <webmaster@nebulon.de>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "pulseaudiodevice.h"

#include "pulseaudioengine.h"

PulseAudioDevice::PulseAudioDevice(PulseAudioDeviceType t, PulseAudioEngine *engine, QObject *parent) :
    QObject(parent),
    m_engine(engine),
    m_volume(0),
    m_mute(false),
    m_type(t)
{
}

PulseAudioDevice::~PulseAudioDevice()
{
}

// this is just for setting the internal volume
void PulseAudioDevice::setVolumeNoCommit(int volume)
{
    if (m_volume == volume)
        return;

    m_volume = volume;
    emit volumeChanged(m_volume);
}

void PulseAudioDevice::toggleMute()
{
    m_engine->setMute(this, !m_mute);
}

void PulseAudioDevice::setMute(bool state)
{
    if (m_mute == state)
        return;

    m_mute = state;
    emit muteChanged();
}

void PulseAudioDevice::increaseVolume()
{
    setVolume(volume()+10);
}

void PulseAudioDevice::decreaseVolume()
{
    setVolume(volume()-10);
}

// this performs a volume change on the device
void PulseAudioDevice::setVolume(int volume)
{
    int tmp = volume;

    if (tmp < 0)
        tmp = 0;
    else if (tmp > (int)PA_VOLUME_UI_MAX)
        tmp = PA_VOLUME_UI_MAX;

    if (m_volume == tmp)
        return;

    m_volume = tmp;

    if (m_engine)
        m_engine->commitDeviceVolume(this);
}
