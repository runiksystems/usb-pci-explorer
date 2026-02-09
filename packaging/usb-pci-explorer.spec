Name:           usb-pci-explorer
Version:        1.0.0
Release:        1%{?dist}
Summary:        USB and PCI topology explorer (RunikSystems)
Group:          Applications/System

License:        GPLv3+
URL:            https://github.com/runiksystems/usb-pci-explorer
Source0:        %{name}-%{version}.tar.gz

# Outils de build nécessaires
BuildRequires:  gcc-c++
BuildRequires:  cmake
BuildRequires:  qt6-qtbase-devel

# Dépendances au runtime pour Fedora/RHEL
Requires:       qt6-qtbase
Requires:       hicolor-icon-theme
Requires:       usbutils
Requires:       pciutils

%description
A lightweight utility developed in C++ and Qt to correlate
USB device topology with their parent PCI controllers.
Developed by Nicolas Dupuis for RunikSystems.
Optimized for Fedora and RHEL-based distributions.

%prep
%autosetup

%build
# Macro standard Fedora pour CMake
%cmake
%cmake_build

%install
# Macro standard Fedora pour l'installation
%cmake_install

%files
# Exécutable
%{_bindir}/usb-pci-explorer

# Intégration Desktop
%{_datadir}/applications/usb-pci-explorer.desktop
%{_datadir}/icons/hicolor/scalable/apps/usb-pci-explorer.svg

# Documentation et Licence
%license LICENSE
%doc CHANGELOG.md README.md

%changelog
* Mon Feb 09 2026 Nicolas Dupuis <59712122+runiksystems@users.noreply.github.com> - 1.0.0
- Initial release for RunikSystems
- Full compatibility with Fedora and RHEL-like systems
- Automated Production RPM Packaging via GitHub Actions

