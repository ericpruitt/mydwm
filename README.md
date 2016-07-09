mydwm
=====

This repository contains patches and configuration options for building
[dwm][dwm] to suit my preferences and needs. All patches in the "patches"
folder starting with "00" can be cleanly applied to dwm independent of any
other patches, and patches with "+" symbols in the file name are patches used
to make patches that could not otherwise coexist work together.

The most important targets supported by the Makefile are:

- **dwm:** Fetch the dwm source code, apply patches and build the executable.
  This is the default target when no targets are specified as command line
  arguments. The file "build.mk" file is copied into the dwm repository as
  "config.mk" before compiling dwm. It was renamed so "config.h" could be
  autocompleted after typing a single character.
- **apply:** Use `patch(1)` to apply all files in the "patches" directory
  ending in ".diff" to the dwm tree.
- **clean:** Revert the dwm source tree using `git reset --hard`.
- **cleaner:** Delete the local copy of the dwm source code repository.
- **sanity:** Verify that all patches starting with "00" can be applied to dwm
  independent of any other patches. Executing this target will **not**
  automatically download the dwm source code.

  [dwm]: http://dwm.suckless.org/

Patch Set
---------

### Standalone ###

#### Better Borders ####

**File:** 00-better-borders.diff

Remove the border when there is just one window visible. Unlike some of the
other dwm border patches, my patch uses a more generic approach that means no
changes need to the made to code of individual layout functions provided the
layout doesn't do things like stack / obscure windows.

#### Click to Focus ####

**File:** 00-click-to-focus.diff

Window focus is only changed by clicking instead of hovering. Unlike the
[focusonclick][focusonclick] patch, the MODKEY mouse actions all remain
functional with this patch applied.

  [focusonclick]: http://dwm.suckless.org/patches/focusonclick

#### Custom Rules Hook ####

**File:** 00-custom-rules-hook.diff

This change allows users to define more complex constraints than what the
"rules" array allows by; the user creates a function named "ruleshook" in
"config.h" that will be executed at the end of "applyrules" for every window
dwm manages.

#### Hide Vacant Tags ####

**File:** 00-hide-vacant-tags.diff

Hides tags that are not selected and contain no windows.

#### Eliminate Reordering Caused By _NET_ACTIVE_WINDOW ####

**File:** 00-no-reordering-from-net-active-window.diff

Some poorly behaved applications (Steam, Chromium) unnecessarily emit a
_NET_ACTIVE_WINDOW event when focused or clicked causing dwm to pop the window
and re-order the window list. This patch changes dwm's behaviour so
_NET_ACTIVE_WINDOW events result in "focus" being called on a window without
popping it to prevent unwanted reordering.

#### Regular Expression Rules ####

**File:** 00-regex-rules.diff

Makes all of the window rule attributes that accept strings use regular
expressions for matching text.

#### Select Previous Window ####

**File:** 00-select-previous-window.diff

Switch focus from the current window to whichever window was previously
selected. This patch adds a timestamp to each window that is set to the current
time whenever a window is focused. The "previous window" is defined as the
window with the highest timestamp of the currently visible but unfocused
windows.

### Implementation of "seturgent" Function ###

**File:** 00-seturgent-function.diff

Adds a new function called "seturgent" for setting XUrgencyHint.

#### SIGHUP to Quit ####

**File:** 00-sighup-to-quit.diff

Adds a signal handler for SIGHUP that causes dwm to shutdown cleanly.

#### Smaller Clickable Tag-Bar Area ####

**File:** 00-smaller-clickable-tag-bar-area.diff

Ensures that mouse actions on the window title area of the status bar are only
registered if they happen in the top half of the tag bar. This is to reduce the
amount of accidental window closures caused by attempts to middle-mouse browser
tabs.

#### System Tray ####

**File:** 00-systray.diff

Implements system tray for dwm.

#### Tag Rules ####

**File:** 00-tag-rules.diff

Adds a new set of rules that can be used to set the layout, nmaster and mfact
values when certain tags are active.

#### Urgent Tag Color ####

**File:** 00-urgent-tag-color.diff

Additional color option to specify the color of tags containing windows with
urgency hints.

#### Visibility Removes Urgency ####

**File:** 00-visibility-removes-urgency.diff

Removes urgency hint from any window that is visible. This will not reset most
applications' internal urgency states which means things like tray icons may
stay active even though a window's urgency hint has been removed.

#### Window Attachment Priority ####

**File:** 00-window-attachment-priority.diff

Adds an additional option to the window rules that allows specification of a
window's attachment priority. A window with a given attachment priority will
always be attached above windows with a lower attachment priority and cannot be
zoomed above a window with a higher priority.

### Dependent ###

#### More Extended Window Manager Hints ####

**File:** 10-more-ewmhs.diff

**Dependencies:**

- 00-no-reordering-from-net-active-window.diff
- 00-systray.diff

This patch adds support for _NET_NUMBER_OF_DESKTOPS, _NET_CURRENT_DESKTOP,
_NET_WM_DESKTOP and _NET_CLOSE_WINDOW so dwm can be manipulated with tools like
[wmctrl][wmctrl]. Because of how dwm tags work, there is not a one-to-one
mapping of tag numbers and desktop numbers. For example, if a window is on tags
1 and 3 (indexed from 1) in dwm, wmctrl will report the window as being on
desktop 5: `2^(1 - 1) + 2^(3 - 1)`.

  [wmctrl]: https://sites.google.com/site/tstyblo/wmctrl/
