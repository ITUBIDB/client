# Kovan Desktop Client

## Introduction

Kovan Desktop Client is a tool to synchronize files from İTÜ Kovan Server
with your computer.

## Download

### Source code

The Kovan Desktop Client is developed in Git. Since Git makes it easy to
fork and improve the source code and to adapt it to your need, many copies
can be found on the Internet, in particular on GitHub. However, the
authoritative repository maintained by the developers is located at
https://github.com/ITUBIDB/kovan-desktop-client.

## Building the source code

## MacOS Building steps (Tested with OS X Catalina)

### Building the client (After fresh OS install)
* Install Xcode from AppStore and start Xcode to complete installation.
* Install Xcode `xcode-select --install`
* Install Brew `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"`
* Add ownCloud repo `brew tap owncloud/owncloud`
* Install qt5, cmake, Openssl 1.1, pkgconfig using `brew install qt5 cmake openssl@1.1 pkgconfig $(brew deps owncloud-client)`
* `export PATH="/usr/local/opt/qt/bin:$PATH" && export LDFLAGS="-L/usr/local/opt/qt/lib" && export CPPFLAGS="-I/usr/local/opt/qt/include"`
* `export PATH="/usr/local/opt/openssl@1.1/bin:$PATH" && export LDFLAGS="$LDFLAGS -L/usr/local/opt/openssl@1.1/lib" && export CPPFLAGS="$CPPFLAGS -I/usr/local/opt/openssl@1.1/include"`
* Install Sparkle `brew cask install sparkle`
* Clone repo `git clone --recursive https://github.com/ITUBIDB/kovan-desktop-client/`
* Init submodules using `cd kovan-desktop-client && git submodule init && git submodule update --recursive`
* Create build folder `mkdir build && cd build`
* Configure cmake options `cmake -DCMAKE_PREFIX_PATH=/usr/local/opt/qt -DCMAKE_INSTALL_PREFIX=~/kovan-desktop-client/build/bin -DCMAKE_BUILD_TYPE=Release -DSPARKLE_LIBRARY=/usr/local/Caskroom/sparkle/1.22.0/Sparkle.framework/ ~/kovan-desktop-client/`
* Start build `make`
* Install to Kovan.app folder `make install`
* Create a symlink for Sparkle library `mkdir ~/Library/Frameworks && ln -s /usr/local/Caskroom/sparkle/1.22.0/Sparkle.framework ~/Library/Frameworks/`
* Copy lib files `sudo cp -a /usr/lib/libz.1.dylib /usr/local/opt/openssl@1.1/lib/libcrypto.1.1.dylib /usr/local/opt/openssl@1.1/lib/libssl.1.1.dylib ~/kovan-desktop-client/build/bin/Kovan.app/Contents/MacOS/`
* Fix framework dependencies `cd ~/kovan-desktop-client/build/bin/Kovan.app/Contents/MacOS/ && sudo -E python ~/kovan-desktop-client/admin/osx/macdeployqt.py ~/kovan-desktop-client/build/bin/Kovan.app /usr/local/opt/qt/bin/qmake && cd ~/kovan-desktop-client/build`
* Fix linking problems `sudo bash ~/kovan-desktop-client/admin/osx/macdeployfixlinks.sh ~/kovan-desktop-client/build/bin/Kovan.app`
* Fix permissions for executables  `chmod 755 ~/kovan-desktop-client/build/bin/Kovan.app/Contents/MacOS/*`
* Fix permissions for frameworks `for dirname in $(ls -1 ~/kovan-desktop-client/build/bin/Kovan.app/Contents/Frameworks); do fname=${dirname//.framework}; find ~/kovan-desktop-client/build/bin/Kovan.app/Contents/Frameworks/${dirname} -name ${fname} -type f -exec chmod 755 {} \; ; done`

### Signing Executables
* Generate and install "Developer ID Application" certificate to login chain with name as "Developer ID Application: Istanbul Teknik Universitesi (XXXXXX)"
* Sign Kovan binary `codesign --deep -o runtime -s "Developer ID Application: Istanbul Teknik Universitesi (XXXXXX)" bin/Kovan.app/Contents/MacOS/Kovan`

### Packaging the client
* Install Packages from http://s.sudre.free.fr/Software/Packages/about.html
* Run `~/kovan-desktop-client/build/admin/osx/create_mac.sh ~/kovan-desktop-client/build/bin/ ~/kovan-desktop-client/build/` to generate pkg, it will be under build folder that's created on cmake configuration step

### Signing the package (Requires a Apple Developer ID code signing certificate installed)
* Install certificate to login chain with name as "Developer ID Installer: Istanbul Teknik Universitesi (XXXXXX)"
* Run `productsign --sign  "Developer ID Installer: Istanbul Teknik Universitesi (XXXXXX)" bin/Kovan-qt5.15.0-2.6.3.0.pkg Kovan-2.6.3.pkg`
* Generate a app-specific password from Apple account. Note the password for notarization.
* Submit signed package to Apple for notarization `xcrun altool --notarize-app --primary-bundle-id "tr.edu.itu.kovan-desktop" --file Kovan-2.6.3.pkg --username "APPLE-DEVELOPER-ID-ACCOUNT NAME" --password "APP-SPECIFIC-PASSWORD"`
* Check notarization status `xcrun altool --notarization-info "REQUEST-UUID" -u "APPLE-DEVELOPER-ID-ACCOUNT NAME"`
* After "Status: success" message, staple the notarization ticket to installer `xcrun stapler staple Kovan-2.6.3.pkg`

## Windows Building steps (Tested with Windows 10 x64)

### Building the client (After fresh OS install)
* Install both Python 2 and Python 3 latest from python.org
* Install Visual Studio 2019 with "Desktop Development with C++"
* After the dependencies are installed, install KDE Craft using the following lines in PowerShell with admin privileges. Use default settings.
  ```powershell
  Set-ExecutionPolicy -Scope CurrentUser RemoteSigned
  iex ((new-object net.webclient).DownloadString('https://raw.githubusercontent.com/KDE/craft/master/setup/install_craft.ps1'))
  ```
* Launch the KDE Craft Environment `C:\CraftRoot\craft\craftenv.ps1`
* Add kovan craft blueprint repository: `craft --add-blueprint-repository https://github.com/ITUBIDB/craft-blueprints-owncloud.git`
* Build the client: `craft --buildtype Release owncloud-client`

### Packaging the client
* Install nsis: `craft --buildtype Release nsis`
* There is mismatch folder name in dependencies. To fix, run following command in powershell
  `Rename-Item C:\CraftRoot\build\libs\runtime\image-RelWithDebInfo-16\ image-Release-16`
* Create package for kovan: `craft --buildtype Release --package owncloud-client`
* Rename setup exe `Get-ChildItem -Path "C:\CraftRoot\tmp\" | Where-Object { $_.Name -like "owncloud-client-Kovan*.exe" } | %{ Rename-Item -LiteralPath $_.FullName -NewName "Kovan-2.6.3.exe" }`

### Signing the package (Requires a code signing certificate, following example is for Global Sign USB Token)
* Install Safenet Drivers from "https://support.globalsign.com/ssl/ssl-certificates-installation/safenet-drivers"
* Plug in USB Token
* Open "Developer Command Prompt" from "Tools" -> "Command Line" in Visual Studio
* Sign the package with this command: `signtool sign /a /tr http://rfc3161timestamp.globalsign.com/advanced /td SHA256 "C:\CraftRoot\tmp\Kovan-2.6.3.exe"`

## Reporting issues and contributing

If you find any bugs or have any suggestion for improvement, please
file an issue at https://yardim.itu.edu.tr Do not
contact the authors directly by mail, as this increases the chance
of your report being lost.

If you created a patch, please submit a [Pull
Request](https://github.com/ITUBIDB/kovan-desktop-client/pulls).

## Maintainers and Contributors

The current maintainers of this repository are:

* ITU-BIDB <sistemdestek@itu.edu.tr>

Kovan Desktop Client is developed by the ownCloud community and branded by ITU BIDB for usage of Istanbul Technical University.

## License

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
    for more details.
