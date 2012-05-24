/*
Copyright 2012 Canonical Ltd.

Authors:
    Charles Kerr <charles.kerr@canonical.com>

This program is free software: you can redistribute it and/or modify it 
under the terms of the GNU General Public License version 3, as published 
by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranties of 
MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR 
PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along 
with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>
#include "device.h"

namespace
{
  void ensure_glib_initialized ()
  {
    static bool initialized = false;

    if (G_UNLIKELY(!initialized))
    {
      initialized = true;
      g_type_init();
    }
  }
}

/***
****
***/

TEST(DeviceTest, GObjectNew)
{
  ensure_glib_initialized ();

  GObject * o = G_OBJECT (g_object_new (INDICATOR_POWER_DEVICE_TYPE, NULL));
  ASSERT_TRUE (o != NULL);
  ASSERT_TRUE (INDICATOR_IS_POWER_DEVICE(o));
  g_clear_pointer (&o, g_object_unref);
}

TEST(DeviceTest, New)
{
  ensure_glib_initialized ();

  IndicatorPowerDevice * device = indicator_power_device_new ("/object/path",
                                                              UP_DEVICE_KIND_BATTERY,
                                                              "icon",
                                                              50.0,
                                                              UP_DEVICE_STATE_CHARGING,
                                                              30);
  ASSERT_TRUE (device != NULL);
  ASSERT_TRUE (INDICATOR_IS_POWER_DEVICE(device));
  ASSERT_EQ (indicator_power_device_get_kind(device), UP_DEVICE_KIND_BATTERY);
  ASSERT_EQ (indicator_power_device_get_state(device), UP_DEVICE_STATE_CHARGING);
  ASSERT_STREQ (indicator_power_device_get_object_path(device), "/object/path");
  ASSERT_STREQ (indicator_power_device_get_icon(device), "icon");
  ASSERT_EQ ((int)indicator_power_device_get_percentage(device), 50);
  ASSERT_EQ (indicator_power_device_get_time(device), 30);

  /* cleanup */
  g_clear_pointer (&device, g_object_unref);
}

TEST(DeviceTest, NewFromVariant)
{
  ensure_glib_initialized ();

  GVariant * variant = g_variant_new ("(susdut)",
                                      "/object/path",
                                      UP_DEVICE_KIND_BATTERY,
                                      "icon",
                                      50.0,
                                      UP_DEVICE_STATE_CHARGING,
                                      30);
  IndicatorPowerDevice * device = indicator_power_device_new_from_variant (variant);
  ASSERT_TRUE (variant != NULL);
  ASSERT_TRUE (device != NULL);
  ASSERT_TRUE (INDICATOR_IS_POWER_DEVICE(device));
  ASSERT_EQ (indicator_power_device_get_kind(device), UP_DEVICE_KIND_BATTERY);
  ASSERT_EQ (indicator_power_device_get_state(device), UP_DEVICE_STATE_CHARGING);
  ASSERT_STREQ (indicator_power_device_get_object_path(device), "/object/path");
  ASSERT_STREQ (indicator_power_device_get_icon(device), "icon");
  ASSERT_EQ ((int)indicator_power_device_get_percentage(device), 50);
  ASSERT_EQ (indicator_power_device_get_time(device), 30);

  /* cleanup */
  g_clear_pointer (&device, g_object_unref);
  g_clear_pointer (&variant, g_variant_unref);
}
