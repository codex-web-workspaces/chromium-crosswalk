# Archived IPTV compatibility changes

This branch archives the existing local Chromium/Crosswalk modifications. Each
modified repository has one new commit named `last commit`; the changes are not
split by feature. This is a source archive, not a claim that this tree builds or
passes browser or Android tests.

## Baselines

| Repository | Baseline before the archive commit |
| --- | --- |
| Chromium 77.0.3865.92 | `dd34c592a78832912275550dc81d6e131098345d` |
| V8 7.7.299.11 | `027689dbfcb2a9bbc8ceec4db2631c558e879633` |
| Crosswalk | `8917f5c93597099db320fab73c2aa31a151a254a` |
| Chromium ICU | `682a230923933a7157a41b88c7804b6b7d2abdfa` |

## Included behavior and implementation work

- X/Y page scaling driven by `page-view-size` metadata and a Java scale callback,
  with changes across CSS, layout, painting, DOM geometry and input coordinates.
- Legacy page styling, focus appearance, frame borders, broken-image resources
  and Android transparency settings.
- Remote-control key mappings, focus navigation, scrolling and nonstandard DOM
  keyboard/default-action handling.
- Java host callbacks for scale, errors, cookies, storage and frame lifecycle;
  global script injection and the `window.src` compatibility script.
- IPTV `MediaPlayer` IDL/bindings and Java forwarding interfaces, including the
  unfinished source files needed to understand the intended interface.
- Proxy configuration, transport-connect timeout, cookie mirroring, optional
  Java storage fallback/mirroring, and navigation/frame/XHR compatibility changes.
- V8 changes to nullish access, exceptions, ARM calls, `indexOf` comparisons,
  URI decoding and the JavaScript clock, plus shared configuration globals.
- Native library loading, popup handling, lifecycle workarounds, diagnostics and
  build/dependency changes; ICU diagnostics/alignment experiments.

Existing tracked edits are preserved without opportunistic fixes or formatting.
New proxy, V8 helper, MediaPlayer, remote-control and Java bridge sources are
included even where their integration is incomplete. A file being archived does
not imply that its feature is enabled in the build.

## Important limitations

- The historical startup path selects single-process mode. V8 globals and direct
  Blink/net-to-Java calls do not provide per-view isolation or a multiprocess API.
- The native MediaPlayer source/IDL/binding/window build entries remain commented
  out. Meanwhile, standard `MediaFactory::CreateMediaPlayer` returns `nullptr`
  and the video-frame extractor has empty paths. These are not a working media
  replacement.
- Two-axis layout conversion is incomplete; shader/compositor scaling, DOM
  geometry, fonts, caching and input behavior are not interchangeable.
- Several changes weaken security boundaries, alter web/JavaScript semantics,
  suppress exceptions or omit callbacks. Preserve them for analysis; do not
  treat the archive as a recommended implementation for another engine.
- The standalone `init.js` files are preserved, but no automatic loading of
  those files was established during source analysis.
- Android 6 support and reduced package size have not been verified by this
  archival operation. No Chromium build or runtime test was performed.

## Submodules and checkout

The main repository pins the modified dependencies as Git submodules:

| Path | Repository |
| --- | --- |
| `v8` | `git@github.com:codex-web-workspaces/v8-crosswalk.git` |
| `xwalk` | `git@github.com:codex-web-workspaces/crosswalk.git` |
| `third_party/icu` | `git@github.com:codex-web-workspaces/chromium-icu.git` |

Use the committed gitlinks as the source of truth, rather than following the
latest commit on a dependency branch:

```sh
git clone --branch iptv-archive --recurse-submodules \
  git@github.com:codex-web-workspaces/chromium-crosswalk.git
git -C chromium-crosswalk submodule status --recursive
```

These three submodules are not all of Chromium's dependencies. Other historical
DEPS/gclient-managed repositories and the old toolchain are still required to
build. The historical Crosswalk DEPS files and generated gclient hooks have not
been redesigned: an unreviewed `gclient sync` may select old revisions or change
remotes. Review that workflow before using it; do not overwrite the archived
submodule commits. Configurations and caches outside this `src` repository are
not included in this commit.

## Deliberately excluded local artifacts

- `v8_bak/` and `v8_change_bak/`: alternative/backup V8 worktrees, not the active
  V8 dependency.
- `components/BUILD.gn_bak` and `content/public/browser/BUILD.gn_bak`: backups.
- `index.html` and `index.html.1`: unrelated downloaded page snapshots.
- `third_party/blink/renderer/core/page/overscroll_controller.cc`: duplicate
  outside the active `core/page/scrolling/` source path.
- `third_party/libpxc/`: unchanged historical RealSense dependency already
  declared by Crosswalk DEPS, not a newly implemented IPTV component.
- Existing ignored build outputs, fetched dependency trees and local caches.

Excluded files are left untouched locally. They are neither deleted nor silently
folded into this archive as additional source dependencies.
