%bcond_without tests
%bcond_without weak_deps

%global __os_install_post %(echo '%{__os_install_post}' | sed -e 's!/usr/lib[^[:space:]]*/brp-python-bytecompile[[:space:]].*$!!g')
%global __provides_exclude_from ^/opt/ros/noetic/.*$
%global __requires_exclude_from ^/opt/ros/noetic/.*$

Name:           ros-noetic-tesseract-scene-graph
Version:        0.4.0
Release:        1%{?dist}%{?release_suffix}
Summary:        ROS tesseract_scene_graph package

License:        Apache 2.0
Source0:        %{name}-%{version}.tar.gz

Requires:       boost-devel
Requires:       boost-python3-devel
Requires:       console-bridge-devel
Requires:       eigen3-devel
Requires:       orocos-kdl-devel
Requires:       ros-noetic-tesseract-common
Requires:       ros-noetic-tesseract-geometry
BuildRequires:  boost-devel
BuildRequires:  boost-python3-devel
BuildRequires:  cmake
BuildRequires:  console-bridge-devel
BuildRequires:  eigen3-devel
BuildRequires:  gtest-devel
BuildRequires:  orocos-kdl-devel
BuildRequires:  ros-noetic-ros-industrial-cmake-boilerplate
BuildRequires:  ros-noetic-tesseract-common
BuildRequires:  ros-noetic-tesseract-geometry
BuildRequires:  ros-noetic-tesseract-support
Provides:       %{name}-devel = %{version}-%{release}
Provides:       %{name}-doc = %{version}-%{release}
Provides:       %{name}-runtime = %{version}-%{release}

%description
The tesseract_scene_graph package

%prep
%autosetup -p1

%build
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/noetic/setup.sh" ]; then . "/opt/ros/noetic/setup.sh"; fi
mkdir -p obj-%{_target_platform} && cd obj-%{_target_platform}
%cmake3 \
    -UINCLUDE_INSTALL_DIR \
    -ULIB_INSTALL_DIR \
    -USYSCONF_INSTALL_DIR \
    -USHARE_INSTALL_PREFIX \
    -ULIB_SUFFIX \
    -DCMAKE_INSTALL_PREFIX="/opt/ros/noetic" \
    -DCMAKE_PREFIX_PATH="/opt/ros/noetic" \
    -DSETUPTOOLS_DEB_LAYOUT=OFF \
    ..

%make_build

%install
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/noetic/setup.sh" ]; then . "/opt/ros/noetic/setup.sh"; fi
%make_install -C obj-%{_target_platform}

%if 0%{?with_tests}
%check
# Look for a Makefile target with a name indicating that it runs tests
TEST_TARGET=$(%__make -qp -C obj-%{_target_platform} | sed "s/^\(test\|check\):.*/\\1/;t f;d;:f;q0")
if [ -n "$TEST_TARGET" ]; then
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/noetic/setup.sh" ]; then . "/opt/ros/noetic/setup.sh"; fi
CTEST_OUTPUT_ON_FAILURE=1 \
    %make_build -C obj-%{_target_platform} $TEST_TARGET || echo "RPM TESTS FAILED"
else echo "RPM TESTS SKIPPED"; fi
%endif

%files
/opt/ros/noetic

%changelog
* Fri Apr 23 2021 Levi Armstrong <levi.armstrong@swri.org> - 0.4.0-1
- Autogenerated by Bloom

* Wed Apr 14 2021 Levi Armstrong <levi.armstrong@swri.org> - 0.3.1-1
- Autogenerated by Bloom

* Fri Apr 09 2021 Levi Armstrong <levi.armstrong@swri.org> - 0.3.0-1
- Autogenerated by Bloom

