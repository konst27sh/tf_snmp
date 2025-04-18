#
# This software is licensed under the Public Domain.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=tf_snmp

PKG_VERSION:= 1.0
PKG_RELEASE:= 2

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
endef

define Package/tf_snmp/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tf_snmp $(1)/usr/bin/
endef

$(eval $(call BuildPackage,tf_snmp))
