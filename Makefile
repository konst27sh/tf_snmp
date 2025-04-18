#
# This software is licensed under the Public Domain.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=tf_snmp

PKG_VERSION:=${shell (grep -oP 'APP_VERSION_MAJOR\s*\(\s*\K\d+' "$(CURDIR)/src/version.h")}
PKG_RELEASE:=${shell (grep -oP 'APP_VERSION_MINOR\s*\(\s*\K\d+' "$(CURDIR)/src/version.h")}

#CMAKE_OPTIONS += \
#    -DPKG_VERSION_MAJOR=$(word 1, $(subst ., ,$(PKG_VERSION))) \
#    -DPKG_VERSION_MINOR=$(word 2, $(subst ., ,$(PKG_VERSION))) \
#    -DPKG_VERSION_PATCH=$(word 3, $(subst ., ,$(PKG_VERSION))) \
#    -DPKG_RELEASE=$(PKG_RELEASE)

ifdef DEBUG
    TARGET_CFLAGS += -DDEBUG
    CMAKE_OPTIONS += -DDEBUG=ON
endif

PKG_MAINTAINER:=Belyaev Alex <abelyaev@fort-telecom.ru>
PKG_LICENSE:=CC0-1.0

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/tf_snmp
	SECTION:=tfortis
	# Select package by default
	DEFAULT:=y
	CATEGORY:=TFortis
	TITLE:=App for communicate with periphery (Sensors, UPS, SFP), includes Daemon and Controller
	DEPENDS:=+libnetsnmp +jansson
	URL:=https://www.tfortis.com
endef

define Package/tf_snmp/description
	App for communicate with periphery (Sensors, UPS, SFP), includes Daemon and Controller
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) ./src/version.c.in $(PKG_BUILD_DIR)/
	$(CP) ./src/version.h.in $(PKG_BUILD_DIR)/
endef

define Package/tf_snmp/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tf_snmp $(1)/usr/bin/
endef

#.PHONY: bump-version
#bump-version:
#	@[ "${VERSION}" ] || ( echo ">> VERSION is not set (format: X.Y.Z)"; exit 1 )
#	@echo "Updating version to $(VERSION)"
#
#	# Обновление Makefile
#	@sed -i -E '/^PKG_VERSION\?=/{s/[0-9]+\.[0-9]+\.[0-9]+/$(VERSION)/}' Makefile
#
#	# Обновление CMake
#	@sed -i -E '/project\(/s/VERSION [0-9]+\.[0-9]+\.[0-9]+/VERSION $(VERSION)/' CMakeLists.txt
#
#	# Обновление релиза (если нужно)
#	@[ "${RELEASE}" ] && sed -i -E '/^PKG_RELEASE\?=/{s/[0-9]+/$(RELEASE)/}' Makefile || true
#
#.PHONY: validate-version
#validate-version:
#	@echo "Current version: ${PKG_VERSION}-${PKG_RELEASE}"
#	@echo "CMake version: ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
#
#.PHONY: tag-version
#tag-version:
#	git tag -a v${PKG_VERSION}-${PKG_RELEASE} -m "Release ${PKG_VERSION}-${PKG_RELEASE}"
#	git push origin v${PKG_VERSION}-${PKG_RELEASE}

$(eval $(call BuildPackage,tf_snmp))
