/* arch/arm/mach-msm/include/mach/board.h
 *
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2008-2012, Code Aurora Forum. All rights reserved.
 * Author: Brian Swetland <swetland@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __ASM_ARCH_MSM_BOARD_H
#define __ASM_ARCH_MSM_BOARD_H

#include <linux/types.h>
#include <asm/setup.h>
#include <linux/input.h>
#include <linux/usb.h>
#include <linux/leds-pmic8058.h>
#include <linux/clkdev.h>
#include <linux/of_platform.h>
#include <linux/msm_ssbi.h>
#include <mach/msm_bus.h>

struct msm_camera_io_ext {
	uint32_t mdcphy;
	uint32_t mdcsz;
	uint32_t appphy;
	uint32_t appsz;
	uint32_t camifpadphy;
	uint32_t camifpadsz;
	uint32_t csiphy;
	uint32_t csisz;
	uint32_t csiirq;
	uint32_t csiphyphy;
	uint32_t csiphysz;
	uint32_t csiphyirq;
	uint32_t ispifphy;
	uint32_t ispifsz;
	uint32_t ispifirq;
};

struct msm_camera_io_clk {
	uint32_t mclk_clk_rate;
	uint32_t vfe_clk_rate;
};

struct msm_cam_expander_info {
	struct i2c_board_info const *board_info;
	int bus_id;
};

struct msm_camera_device_platform_data {
	int (*camera_gpio_on) (void);
	void (*camera_gpio_off)(void);
	struct msm_camera_io_ext ioext;
	struct msm_camera_io_clk ioclk;
	uint8_t csid_core;
	int (*camera_csi_on) (void);
	int (*camera_csi_off) (void);
	struct msm_bus_scale_pdata *cam_bus_scale_table;
};
enum msm_camera_csi_data_format {
	CSI_8BIT,
	CSI_10BIT,
	CSI_12BIT,
};
struct msm_camera_csi_params {
	enum msm_camera_csi_data_format data_format;
	uint8_t lane_cnt;
	uint8_t lane_assign;
	uint8_t settle_cnt;
	uint8_t dpcm_scheme;
	uint8_t mipi_driving_strength;/*from 0-3*/
	uint8_t hs_impedence;
};

#ifdef CONFIG_SENSORS_MT9T013
struct msm_camera_legacy_device_platform_data {
	int sensor_reset;
	int sensor_pwd;
	int vcm_pwd;
	void (*config_gpio_on) (void);
	void (*config_gpio_off)(void);
};
#endif

#define MSM_CAMERA_FLASH_NONE 0
#define MSM_CAMERA_FLASH_LED  1

#define MSM_CAMERA_FLASH_SRC_PMIC (0x00000001<<0)
#define MSM_CAMERA_FLASH_SRC_PWM  (0x00000001<<1)
#define MSM_CAMERA_FLASH_SRC_CURRENT_DRIVER	(0x00000001<<2)
#define MSM_CAMERA_FLASH_SRC_EXT     (0x00000001<<3)


struct msm_camera_sensor_flash_pmic {
	uint8_t num_of_src;
	uint32_t low_current;
	uint32_t high_current;
	enum pmic8058_leds led_src_1;
	enum pmic8058_leds led_src_2;
	int (*pmic_set_current)(enum pmic8058_leds id, unsigned mA);
};

struct msm_camera_sensor_flash_pwm {
	uint32_t freq;
	uint32_t max_load;
	uint32_t low_load;
	uint32_t high_load;
	uint32_t channel;
};

struct pmic8058_leds_platform_data;
struct msm_camera_sensor_flash_current_driver {
	uint32_t low_current;
	uint32_t high_current;
	const struct pmic8058_leds_platform_data *driver_channel;
};

struct msm_camera_sensor_flash_external {
	uint32_t led_en;
	uint32_t led_flash_en;
	struct msm_cam_expander_info *expander_info;
};

struct msm_camera_sensor_flash_src {
	int flash_sr_type;
	int (*camera_flash)(int level);

	union {
		struct msm_camera_sensor_flash_pmic pmic_src;
		struct msm_camera_sensor_flash_pwm pwm_src;
		struct msm_camera_sensor_flash_current_driver
			current_driver_src;
		struct msm_camera_sensor_flash_external
			ext_driver_src;
	} _fsrc;
};

struct msm_camera_sensor_flash_data {
	int flash_type;
	struct msm_camera_sensor_flash_src *flash_src;
};

/* Andrew_Cheng linear led 20111205 MB*/
struct camera_led_info {
	uint16_t enable;
	uint16_t low_limit_led_state;
	uint16_t max_led_current_ma;
	uint16_t num_led_est_table;
};

struct camera_led_est {
	uint16_t enable;
	uint16_t led_state;
	uint16_t current_ma;
	uint16_t lumen_value;
	uint16_t min_step;
	uint16_t max_step;
};

struct camera_flash_info {
	struct camera_led_info *led_info;
	struct camera_led_est *led_est_table;
};
/* Andrew_Cheng linear led 20111205 ME */

struct camera_flash_cfg {
	int num_flash_levels;
	int (*camera_flash)(int level);
	uint16_t low_temp_limit;
	uint16_t low_cap_limit;
	uint16_t low_cap_limit_dual;
	uint8_t postpone_led_mode;
	struct camera_flash_info *flash_info;	/* Andrew_Cheng linear led 20111205 */
};

struct msm_camera_sensor_strobe_flash_data {
	uint8_t flash_trigger;
	uint8_t flash_charge; /* pin for charge */
	uint8_t flash_charge_done;
	uint32_t flash_recharge_duration;
	uint32_t irq;
	spinlock_t spin_lock;
	spinlock_t timer_lock;
	int state;
};

struct msm_camera_rawchip_info {
	int rawchip_reset;
	int rawchip_intr0;
	int rawchip_intr1;
	uint8_t rawchip_spi_freq;
	uint8_t rawchip_mclk_freq;
	int (*camera_rawchip_power_on)(void);
	int (*camera_rawchip_power_off)(void);
	int (*rawchip_use_ext_1v2)(void);
};

enum msm_camera_type {
	BACK_CAMERA_2D,
	FRONT_CAMERA_2D,
	BACK_CAMERA_3D,
	BACK_CAMERA_INT_3D,
};

struct msm8960_privacy_light_cfg {
	unsigned mpp;
};

enum sensor_flip_mirror_info {
	CAMERA_SENSOR_NONE,
	CAMERA_SENSOR_MIRROR,
	CAMERA_SENSOR_FLIP,
	CAMERA_SENSOR_MIRROR_FLIP,
};

struct msm_camera_sensor_platform_info {
	int mount_angle;
	int sensor_reset_enable;
	int sensor_reset;
	int sensor_pwd;
	int vcm_pwd;
	int vcm_enable;
	int privacy_light;
	enum sensor_flip_mirror_info mirror_flip;
	void *privacy_light_info;
};

struct msm_camera_gpio_conf {
	void *cam_gpiomux_conf_tbl;
	uint8_t cam_gpiomux_conf_tbl_size;
	uint16_t *cam_gpio_tbl;
	uint8_t cam_gpio_tbl_size;
};

struct msm_actuator_info {
	struct i2c_board_info const *board_info;
	int bus_id;
	int vcm_pwd;
	int vcm_enable;
	int use_rawchip_af;
};

enum msm_camera_platform{
	MSM_CAMERA_PLTFORM_8X60	= 0,
	MSM_CAMERA_PLTFORM_7X30	= 1,
	MSM_CAMERA_PLTFORM_MAX	= 2,
};

struct msm_camera_sensor_info {
	const char *sensor_name;
	int sensor_reset_enable;
	int sensor_reset;
	int sensor_pwd;
	int vcm_pwd;
	int vcm_enable;
	int mclk;
	int flash_type;
	int need_suspend;
	struct msm_camera_sensor_platform_info *sensor_platform_info;
	struct msm_camera_device_platform_data *pdata;
	struct resource *resource;
	uint8_t num_resources;
	struct msm_camera_sensor_flash_data *flash_data;
	int csi_if;
	struct msm_camera_csi_params csi_params;
	struct msm_camera_sensor_strobe_flash_data *strobe_flash_data;
	char *eeprom_data;
	struct msm_camera_gpio_conf *gpio_conf;
	enum msm_camera_type camera_type;
	struct msm_actuator_info *actuator_info;
	int (*camera_power_on)(void);
	int (*camera_power_off)(void);
	int use_rawchip;
#if 1 /* HTC to be removed */
	/* HTC++ */
	void(*camera_clk_switch)(void);
	int power_down_disable; /* if close power */
	int full_size_preview; /* if use full-size preview */
	int cam_select_pin; /* for two sensors */
	int mirror_mode; /* for sensor upside down */
	int(*camera_pm8058_power)(int); /* for express */
	struct camera_flash_cfg* flash_cfg;
	int gpio_set_value_force; /*true: force to set gpio  */
	int dev_node;
	int camera_platform;
	uint8_t led_high_enabled;
	uint32_t kpi_sensor_start;
	uint32_t kpi_sensor_end;
	uint8_t (*preview_skip_frame)(void);
#endif
};

int  msm_get_cam_resources(struct msm_camera_sensor_info *);

struct clk_lookup;

struct snd_endpoint {
	int id;
	const char *name;
};

struct msm_snd_endpoints {
	struct snd_endpoint *endpoints;
	unsigned num;
};

#define MSM_MAX_DEC_CNT 14
/* 7k target ADSP information */
/* Bit 23:0, for codec identification like mp3, wav etc *
 * Bit 27:24, for mode identification like tunnel, non tunnel*
 * bit 31:28, for operation support like DM, DMA */
enum msm_adspdec_concurrency {
	MSM_ADSP_CODEC_WAV = 0,
	MSM_ADSP_CODEC_ADPCM = 1,
	MSM_ADSP_CODEC_MP3 = 2,
	MSM_ADSP_CODEC_REALAUDIO = 3,
	MSM_ADSP_CODEC_WMA = 4,
	MSM_ADSP_CODEC_AAC = 5,
	MSM_ADSP_CODEC_RESERVED = 6,
	MSM_ADSP_CODEC_MIDI = 7,
	MSM_ADSP_CODEC_YADPCM = 8,
	MSM_ADSP_CODEC_QCELP = 9,
	MSM_ADSP_CODEC_AMRNB = 10,
	MSM_ADSP_CODEC_AMRWB = 11,
	MSM_ADSP_CODEC_EVRC = 12,
	MSM_ADSP_CODEC_WMAPRO = 13,
	MSM_ADSP_MODE_TUNNEL = 24,
	MSM_ADSP_MODE_NONTUNNEL = 25,
	MSM_ADSP_MODE_LP = 26,
	MSM_ADSP_OP_DMA = 28,
	MSM_ADSP_OP_DM = 29,
};

struct msm_adspdec_info {
	const char *module_name;
	unsigned module_queueid;
	int module_decid; /* objid */
	unsigned nr_codec_support;
};

/* Carries information about number codec
 * supported if same codec or different codecs
 */
struct dec_instance_table {
	uint8_t max_instances_same_dec;
	uint8_t max_instances_diff_dec;
};

struct msm_adspdec_database {
	unsigned num_dec;
	unsigned num_concurrency_support;
	unsigned int *dec_concurrency_table; /* Bit masked entry to *
					      *	represents codec, mode etc */
	struct msm_adspdec_info  *dec_info_list;
	struct dec_instance_table *dec_instance_list;
};

enum msm_mdp_hw_revision {
	MDP_REV_20 = 1,
	MDP_REV_22,
	MDP_REV_30,
	MDP_REV_303,
	MDP_REV_31,
	MDP_REV_40,
	MDP_REV_41,
	MDP_REV_42,
	MDP_REV_43,
	MDP_REV_44,
};

struct msm_panel_common_pdata {
	uintptr_t hw_revision_addr;
	int gpio;
	int (*backlight_level)(int level, int max, int min);
	int (*pmic_backlight)(int level);
	int (*rotate_panel)(void);
	int (*panel_num)(void);
	void (*panel_config_gpio)(int);
	int (*vga_switch)(int select_vga);
	int *gpio_num;
	int mdp_core_clk_rate;
	unsigned num_mdp_clk;
	int *mdp_core_clk_table;
	int (*rgb_format)(void);
	unsigned char (*shrink_pwm)(int val);
#ifdef CONFIG_MSM_BUS_SCALING
	struct msm_bus_scale_pdata *mdp_bus_scale_table;
#endif
	int mdp_rev;
	u32 ov0_wb_size;  /* overlay0 writeback size */
	u32 ov1_wb_size;  /* overlay1 writeback size */
	u32 mem_hid;
        char cont_splash_enabled;
	int (*writeback_offset)(void);
	int (*mdp_color_enhance)(void);
	int (*mdp_gamma)(void);
	void (*mdp_img_stick_wa)(bool);
	unsigned long update_interval;
	atomic_t img_stick_on;
	struct panel_dcr_info *dcr_panel_pinfo;
	unsigned int auto_bkl_stat;
	int (*bkl_enable)(int);
	int (*acl_enable)(int);
};

struct lcdc_platform_data {
	int (*lcdc_gpio_config)(int on);
	int (*lcdc_power_save)(int);
	unsigned int (*lcdc_get_clk)(void);
#ifdef CONFIG_MSM_BUS_SCALING
	struct msm_bus_scale_pdata *bus_scale_table;
#endif
};

struct tvenc_platform_data {
	int poll;
	int (*pm_vid_en)(int on);
#ifdef CONFIG_MSM_BUS_SCALING
	struct msm_bus_scale_pdata *bus_scale_table;
#endif
};

struct mddi_platform_data {
	int (*mddi_power_save)(int on);
	int (*mddi_sel_clk)(u32 *clk_rate);
	int (*mddi_client_power)(u32 client_id);
};

struct mipi_dsi_platform_data {
	int vsync_gpio;
	int (*dsi_power_save)(int on);
	int (*esd_fixup)(uint32_t mfd_data);
	int (*dsi_client_reset)(void);
	int (*get_lane_config)(void);
        char (*splash_is_enabled)(void);
	int target_type;
};

enum mipi_dsi_3d_ctrl {
	FPGA_EBI2_INTF,
	FPGA_SPI_INTF,
};

#ifndef CONFIG_ARCH_MSM8X60
/* DSI PHY configuration */
struct mipi_dsi_phy_ctrl {
	uint32_t regulator[5];
	uint32_t timing[12];
	uint32_t ctrl[4];
	uint32_t strength[4];
	uint32_t pll[21];
};
#endif

struct mipi_dsi_panel_platform_data {
	int fpga_ctrl_mode;
	int fpga_3d_config_addr;
	int *gpio;
	struct mipi_dsi_phy_ctrl *phy_ctrl_settings;
};

#define PANEL_NAME_MAX_LEN 50

struct msm_fb_platform_data {
	int (*detect_client)(const char *name);
	int mddi_prescan;
	int (*allow_set_offset)(void);
	int blt_mode;
	uint32_t width;
	uint32_t height;
        char prim_panel_name[PANEL_NAME_MAX_LEN];
        char ext_panel_name[PANEL_NAME_MAX_LEN];
};
#define HDMI_VFRMT_640x480p60_4_3 0
#define HDMI_VFRMT_720x480p60_16_9 2
#define HDMI_VFRMT_1280x720p60_16_9 3
#define HDMI_VFRMT_720x576p50_16_9 17
#define HDMI_VFRMT_1920x1080p24_16_9 31
#define HDMI_VFRMT_1920x1080p30_16_9 33

typedef struct
{
	uint8_t format;
	uint8_t reg_a3;
	uint8_t reg_a6;
}mhl_driving_params;

struct msm_hdmi_platform_data {
	int irq;
	int (*cable_detect)(int insert);
	int (*comm_power)(int on, int show);
	int (*enable_5v)(int on);
	int (*core_power)(int on, int show);
	int (*cec_power)(int on);
	int (*init_irq)(void);
	bool (*check_hdcp_hw_support)(void);
	mhl_driving_params *driving_params;
	int dirving_params_count;
};

struct msm_i2c_platform_data {
	int clk_freq;
	uint32_t rmutex;
	const char *rsl_id;
	uint32_t pm_lat;
	int pri_clk;
	int pri_dat;
	int aux_clk;
	int aux_dat;
	const char *clk;
	const char *pclk;
	int src_clk_rate;
	int use_gsbi_shared_mode;
	void (*msm_i2c_config_gpio)(int iface, int config_type);
	int share_uart_flag;
};

struct msm_i2c_ssbi_platform_data {
	const char *rsl_id;
	enum msm_ssbi_controller_type controller_type;
};

struct msm_vidc_platform_data {
	int memtype;
	u32 enable_ion;
	int disable_dmx;
	int disable_fullhd;
	u32 cp_enabled;
#ifdef CONFIG_MSM_BUS_SCALING
	struct msm_bus_scale_pdata *vidc_bus_client_pdata;
#endif
	int cont_mode_dpb_count;
};

#if defined(CONFIG_USB_PEHCI_HCD) || defined(CONFIG_USB_PEHCI_HCD_MODULE)
struct isp1763_platform_data {
	unsigned reset_gpio;
	int (*setup_gpio)(int enable);
};
#endif

#define SHIP_BUILD	0
#define MFG_BUILD	1
#define ENG_BUILD	2

/* common init routines for use by arch/arm/mach-msm/board-*.c */
#ifdef CONFIG_OF_DEVICE
void msm_copper_init(struct of_dev_auxdata **);
#endif
void  msm_add_devices(void);
void msm_copper_add_devices(void);
void  msm_map_common_io(void);
void  msm_map_qsd8x50_io(void);
void  msm_map_msm8x60_io(void);
void  msm_map_msm8960_io(void);
void  msm_map_msm8930_io(void);
void  msm_map_apq8064_io(void);
void  msm_map_msm7x30_io(void);
void  msm_map_fsm9xxx_io(void);
void msm_map_copper_io(void);
void  msm_init_irq(void);
void vic_handle_irq(struct pt_regs *regs);
void msm_copper_init_irq(void);

struct mmc_platform_data;
int  msm_add_sdcc(unsigned int controller,
		struct mmc_platform_data *plat);

struct msm_usb_host_platform_data;
int  msm_add_host(unsigned int host,
		struct msm_usb_host_platform_data *plat);
#if defined(CONFIG_USB_FUNCTION_MSM_HSUSB) \
	|| defined(CONFIG_USB_MSM_72K) \
	|| defined(CONFIG_USB_MSM_72K_MODULE) \
	|| defined(CONFIG_USB_CI13XXX_MSM)
int usb_get_connect_type(void);
void msm_otg_set_vbus_state(int online);
enum usb_connect_type {
	CONNECT_TYPE_CLEAR = -2,
	CONNECT_TYPE_UNKNOWN = -1,
	CONNECT_TYPE_NONE = 0,
	CONNECT_TYPE_USB,
	CONNECT_TYPE_AC,
	CONNECT_TYPE_9V_AC,
	CONNECT_TYPE_WIRELESS,
	CONNECT_TYPE_INTERNAL,
	CONNECT_TYPE_UNSUPPORTED,
#ifdef CONFIG_MACH_VERDI_LTE
	/* Y cable with USB and 9V charger */
	CONNECT_TYPE_USB_9V_AC,
#endif
};

#else
static inline void msm_otg_set_vbus_state(int online) {}
#endif

/* START: add USB connected notify function */
struct t_usb_status_notifier{
	struct list_head notifier_link;
	const char *name;
	void (*func)(int cable_type);
};
int usb_register_notifier(struct t_usb_status_notifier *notifer);
static LIST_HEAD(g_lh_usb_notifier_list);

/***********************************
Direction: cable detect drvier -> battery driver or other
***********************************/
struct t_cable_status_notifier{
	struct list_head cable_notifier_link;
	const char *name;
	void (*func)(int cable_type);
};
int cable_detect_register_notifier(struct t_cable_status_notifier *);
static LIST_HEAD(g_lh_calbe_detect_notifier_list);

/***********************************
Direction: 1-wire drvier -> battery driver or other
***********************************/
struct t_owe_charging_notifier{
	struct list_head owe_charging_notifier_link;
	const char *name;
	void (*func)(int charging_type);
};
int owe_charging_register_notifier(struct t_owe_charging_notifier *);
static LIST_HEAD(g_lh_owe_charging_notifier_list);

/***********************************
 Direction: sii9234 drvier -> cable detect driver
***********************************/
struct t_mhl_status_notifier{
	struct list_head mhl_notifier_link;
	const char *name;
	void (*func)(bool isMHL, int charging_type);
};
int mhl_detect_register_notifier(struct t_mhl_status_notifier *);
static LIST_HEAD(g_lh_mhl_detect_notifier_list);

#ifdef CONFIG_USB_OTG
/***********************************
Direction: cable detect drvier -> usb driver
 ***********************************/
struct t_usb_host_status_notifier{
	struct list_head usb_host_notifier_link;
	const char *name;
	void (*func)(bool cable_in);
};
int usb_host_detect_register_notifier(struct t_usb_host_status_notifier *);
static LIST_HEAD(g_lh_usb_host_detect_notifier_list);
#endif
/* END: add USB connected notify function */

int board_mfg_mode(void);
int board_build_flag(void);
int __init parse_tag_skuid(const struct tag *tags);
int __init parse_tag_engineerid(const struct tag *tags);
int __init parse_tag_cam(const struct tag *tag);
void msm_snddev_init(void);
void msm_snddev_init_timpani(void);
void msm_snddev_poweramp_on(void);
void msm_snddev_poweramp_off(void);
void msm_snddev_hsed_voltage_on(void);
void msm_snddev_hsed_voltage_off(void);
void msm_snddev_tx_route_config(void);
void msm_snddev_tx_route_deconfig(void);

extern unsigned int msm_shared_ram_phys; /* defined in arch/arm/mach-msm/io.c */

extern int emmc_partition_read_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data);

extern int dying_processors_read_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data);
extern int get_partition_num_by_name(char *name);
#endif
