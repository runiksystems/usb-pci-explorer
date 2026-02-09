# **USB-PCI Explorer**

**USB-PCI Explorer** is a lightweight desktop utility built with C++ and Qt6, designed to simplify hardware topology mapping on Linux systems.

## **The Story Behind the Project**

As a virtualization enthusiast frequently working with **KVM/QEMU**, I often found myself needing to perform **PCI passthrough** for USB controllers or specific ports to virtual machines.

The manual process of correlating output from lsusb and lspci to identify which physical USB port belongs to which PCI bus segment is tedious, error-prone, and repetitive. To solve this friction point, I developed this application to provide an instant, visual correlation of the hardware tree.

What started as a tool to streamline my hypervisor configuration is now an open-source project, with a CLI version planned for future releases to support headless server environments.

## **Features**

* **Real-time Correlation**: Instantly see which USB devices are under which PCI Host Controller.
* **Hardware Topology**: Visualizes the relationship between the USB bus and the PCI bridge.
* **Qt6 Interface**: A clean and modern GUI for desktop environments.
* **Native Packaging**: Fully compatible with Fedora and RHEL-based distributions via RPM.

## **Prerequisites**

To run or build this application, the following system tools are required:

* **usbutils**: Provides the lsusb engine.
* **pciutils**: Provides the lspci engine.
* **Qt6 Libraries**: Specifically qt6-qtbase.

## **Installation**

### **From RPM (Recommended for Fedora/RHEL)**

If you have downloaded the .rpm package from the [Releases](https://github.com/runiksystems/usb-pci-explorer/releases) page:
```shell
sudo dnf install ./usb-pci-explorer-1.0-1.fcXX.x86_64.rpm
```

### **From Source**

1. **Install dependencies**:
```shell
sudo dnf install gcc-c++ cmake qt6-qtbase-devel usbutils pciutils
```

2. **Build**:
```shell
mkdir build && cd build
cmake ..
make
```

3. **Run**:
```shel
./usb-pci-explorer
```
## **Roadmap**

* [x] Initial GUI Release (v1.0)
* [ ] CLI Version for headless servers
* [ ] Export mapping to XML/JSON for KVM XML configuration
* [ ] Interactive Port Highlighting

## **License**

This project is licensed under the **GNU GPLv3** - see the [LICENSE](https://raw.githubusercontent.com/runiksystems/usb-pci-explorer/refs/heads/main/LICENSE) file for details.

*Developed by **Nicolas Dupuis** - **RunikSystems**.*
