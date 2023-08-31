include $(TOPDIR)/rules.mk

PKG_NAME:=openvpn-ubus
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/openvpn-ubus
	DEPENDS:=+libubus +libubox
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=UBUS module for getting information about openVPN server
endef

define Package/openvpn-ubus/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/config

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/openvpn_ubus $(1)/usr/bin/openvpn_ubus
	$(INSTALL_BIN) ./files/openvpn_ubus.init $(1)/etc/init.d/openvpn_ubus
	$(INSTALL_BIN) ./files/openvpn_ubus.config $(1)/etc/config/openvpn_ubus
	$(INSTALL_BIN) ./files/openvpn_ubus_script $(1)/etc/init.d/openvpn_ubus_script
endef

$(eval $(call BuildPackage,openvpn-ubus))