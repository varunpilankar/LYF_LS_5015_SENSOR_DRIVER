/*============================================================================

  Copyright (c) 2014 Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

============================================================================*/
#include <stdio.h>
#include "t4kb3_lib.h"
#include <utils/Log.h>

#define SENSOR_MODEL_NO_T4KB3_8939 "t4kb3"
#define T4KB3_8939_LOAD_CHROMATIX(n) \
  "libchromatix_"SENSOR_MODEL_NO_T4KB3_8939"_"#n".so"


#define FEATURE_CAPTURE_PREVIEW_RAW_IMAGE 0 //0 normal 1 preview

static volatile int32_t prev_gain = 128;//406;
static volatile int32_t current_gain = 0;
static volatile int32_t global_gain = 0;
static sensor_lib_t sensor_lib_ptr;

static struct msm_sensor_power_setting power_setting[] = {
  {
    .seq_type = SENSOR_VREG,
    .seq_val = CAM_VIO,
    .config_val = 0,
    .delay = 1,
  },
/*
  {
    .seq_type = SENSOR_VREG,
    .seq_val = CAM_VANA,
    .config_val = 0,
    .delay = 1,
  },
  {
    .seq_type = SENSOR_VREG,
    .seq_val = CAM_VDIG,
    .config_val = 0,
    .delay = 1,
   },
*/
  {
    .seq_type = SENSOR_GPIO,
	.seq_val = SENSOR_GPIO_VANA,
	.config_val = GPIO_OUT_HIGH,
	.delay = 1,
  },

  {
    .seq_type = SENSOR_GPIO,
    .seq_val = SENSOR_GPIO_VDIG,
    .config_val = GPIO_OUT_HIGH,
    .delay = 1,
  },
  {
    .seq_type = SENSOR_VREG,
    .seq_val = CAM_VAF,
    .config_val = 0,
    .delay = 5,
  },
  {
    .seq_type = SENSOR_GPIO,
    .seq_val = SENSOR_GPIO_STANDBY,
    .config_val = GPIO_OUT_LOW,
    .delay = 1,
  },
  {
    .seq_type = SENSOR_GPIO,
    .seq_val = SENSOR_GPIO_STANDBY,
    .config_val = GPIO_OUT_HIGH,
    .delay = 5,
  },
  {
    .seq_type = SENSOR_GPIO,
    .seq_val = SENSOR_GPIO_RESET,
    .config_val = GPIO_OUT_LOW,
    .delay = 5,
  },
  {
    .seq_type = SENSOR_GPIO,
    .seq_val = SENSOR_GPIO_RESET,
    .config_val = GPIO_OUT_HIGH,
    .delay = 10,
  },
  {
    .seq_type = SENSOR_GPIO,
    .seq_val = SENSOR_GPIO_AF_PWDM,
    .config_val = GPIO_OUT_LOW,
    .delay = 5,
  },
  {
    .seq_type = SENSOR_GPIO,
    .seq_val = SENSOR_GPIO_AF_PWDM,
    .config_val = GPIO_OUT_HIGH,
    .delay = 5,
  },
  {
    .seq_type = SENSOR_CLK,
    .seq_val = SENSOR_CAM_MCLK,
    .config_val = 24000000,
    .delay = 10,
  },
  {
    .seq_type = SENSOR_I2C_MUX,
    .seq_val = 0,
    .config_val = 0,
    .delay = 0,
  },
};

static struct msm_sensor_power_setting power_down_setting[] = {
  {
    .seq_type = SENSOR_VREG,
    .seq_val = CAM_VIO,
    .config_val = 0,
    .delay = 1,
  },
/*
  {
    .seq_type = SENSOR_VREG,
    .seq_val = CAM_VANA,
    .config_val = 0,
    .delay = 1,
  },
  {
    .seq_type = SENSOR_VREG,
    .seq_val = CAM_VDIG,
    .config_val = 0,
    .delay = 1,
   },
*/
  {
    .seq_type = SENSOR_GPIO,
	.seq_val = SENSOR_GPIO_VANA,
	.config_val = GPIO_OUT_LOW,
	.delay = 1,
  },

  {
    .seq_type = SENSOR_GPIO,
    .seq_val = SENSOR_GPIO_VDIG,
    .config_val = GPIO_OUT_LOW,
    .delay = 1,
  },
  {
    .seq_type = SENSOR_VREG,
    .seq_val = CAM_VAF,
    .config_val = 0,
    .delay = 5,
  },
  {
    .seq_type = SENSOR_GPIO,
    .seq_val = SENSOR_GPIO_STANDBY,
    .config_val = GPIO_OUT_LOW,
    .delay = 1,
  },
  {
    .seq_type = SENSOR_GPIO,
    .seq_val = SENSOR_GPIO_RESET,
    .config_val = GPIO_OUT_LOW,
    .delay = 5,
  },
  {
    .seq_type = SENSOR_GPIO,
    .seq_val = SENSOR_GPIO_AF_PWDM,
    .config_val = GPIO_OUT_LOW,
    .delay = 5,
  },
  {
    .seq_type = SENSOR_CLK,
    .seq_val = SENSOR_CAM_MCLK,
    .config_val = 24000000,
    .delay = 10,
  },
  {
    .seq_type = SENSOR_I2C_MUX,
    .seq_val = 0,
    .config_val = 0,
    .delay = 0,
  },
};


static struct msm_camera_sensor_slave_info sensor_slave_info = {
  /* Camera slot where this camera is mounted */
  .camera_id = CAMERA_0,
  /* sensor slave address */
  .slave_addr = 0x6c,
  /* sensor address type */
  .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
  /* sensor id info*/
  .sensor_id_info = {
    /* sensor id register address */
    .sensor_id_reg_addr = 0x0000,
    /* sensor id */
    .sensor_id = 0x1c50,
  },
  /* power up / down setting */
  .power_setting_array = {
    .power_setting = power_setting,
    .size = ARRAY_SIZE(power_setting),
    .power_down_setting = power_down_setting,
    .size_down = ARRAY_SIZE(power_down_setting),
  },
};

static struct msm_sensor_init_params sensor_init_params = {
  .modes_supported = 0,
  .position = 0,
  .sensor_mount_angle = 90,
};

static sensor_output_t sensor_output = {
  .output_format = SENSOR_BAYER,
  .connection_mode = SENSOR_MIPI_CSI,
  .raw_output = SENSOR_10_BIT_DIRECT,
};

static struct msm_sensor_output_reg_addr_t output_reg_addr = {
    .x_output = 0x034C,
    .y_output = 0x034E,
    .line_length_pclk = 0x0342,
    .frame_length_lines = 0x0340,
};

static struct msm_sensor_exp_gain_info_t exp_gain_info = {
   .coarse_int_time_addr = 0x0202,
   .global_gain_addr = 0x0234,//0x0204, linear
   .vert_offset = 6,
};

static sensor_aec_data_t aec_info = {
  .max_gain = 15.99,
  .max_linecount = 30834,
};

static sensor_lens_info_t default_lens_info = {
  .focal_length = 2.96,
  .pix_size = 1.12,
  .f_number = 2.0,
  .total_f_dist = 1.2,
  .hor_view_angle = 62.8,
  .ver_view_angle = 49.3,
};

#ifndef VFE_40
static struct csi_lane_params_t csi_lane_params = {
  .csi_lane_assign = 0xe4,
  .csi_lane_mask = 0xf,
  .csi_if = 1,
  .csid_core = {0},
  .csi_phy_sel = 0,
};
#else
static struct csi_lane_params_t csi_lane_params = {
  .csi_lane_assign = 0x4320,
  .csi_lane_mask = 0x1f,
  .csi_if = 1,
  .csid_core = {0},
  .csi_phy_sel = 0,
};
#endif

static struct msm_camera_i2c_reg_setting init_reg_setting[] = {
   {
    .reg_setting = init_reg_array0,
    .size = ARRAY_SIZE(init_reg_array0),
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .data_type = MSM_CAMERA_I2C_BYTE_DATA,
    .delay = 20,
  },
  {
    .reg_setting = init_reg_array1,
    .size = ARRAY_SIZE(init_reg_array1),
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .data_type = MSM_CAMERA_I2C_BYTE_DATA,
    .delay = 20,
  },
};
static struct sensor_lib_reg_settings_array init_settings_array = {
  .reg_settings = init_reg_setting,
  .size = ARRAY_SIZE(init_reg_setting),
};

static struct msm_camera_i2c_reg_array start_reg_array[] = {
  {0x0100, 0x01},
};

static  struct msm_camera_i2c_reg_setting start_settings = {
  .reg_setting = start_reg_array,
  .size = ARRAY_SIZE(start_reg_array),
  .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
  .data_type = MSM_CAMERA_I2C_BYTE_DATA,
  .delay = 0,
};

static struct msm_camera_i2c_reg_array stop_reg_array[] = {
  {0x0100, 0x00},
};

static struct msm_camera_i2c_reg_setting stop_settings = {
  .reg_setting = stop_reg_array,
  .size = ARRAY_SIZE(stop_reg_array),
  .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
  .data_type = MSM_CAMERA_I2C_BYTE_DATA,
  .delay = 0,
};

static struct msm_camera_i2c_reg_array groupon_reg_array[] = {
  { 0x0104, 0x01 },
};

static struct msm_camera_i2c_reg_setting groupon_settings = {
  .reg_setting = groupon_reg_array,
  .size = ARRAY_SIZE(groupon_reg_array),
  .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
  .data_type = MSM_CAMERA_I2C_BYTE_DATA,
  .delay = 0,
};

static struct msm_camera_i2c_reg_array groupoff_reg_array[] = {
  { 0x0104, 0x00 },
};

static struct msm_camera_i2c_reg_setting groupoff_settings = {
  .reg_setting = groupoff_reg_array,
  .size = ARRAY_SIZE(groupoff_reg_array),
  .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
  .data_type = MSM_CAMERA_I2C_BYTE_DATA,
  .delay = 0,
};

static struct msm_camera_csid_vc_cfg t4kb3_cid_cfg[] = {
  {0, CSI_RAW10, CSI_DECODE_10BIT},
  {1, CSI_EMBED_DATA, CSI_DECODE_8BIT},
};

static struct msm_camera_csi2_params t4kb3_csi_params = {
  .csid_params = {
    .lane_cnt = 4,
    .lut_params = {
      .num_cid = 2,
      .vc_cfg = {
         &t4kb3_cid_cfg[0],
         &t4kb3_cid_cfg[1],
      },
    },
  },
  .csiphy_params = {
    .lane_cnt = 4,
    .settle_cnt = 0x14,
  },
};

static struct msm_camera_csi2_params *csi_params[] = {
#if FEATURE_CAPTURE_PREVIEW_RAW_IMAGE
  &t4kb3_csi_params, /* RES 1*/
  &t4kb3_csi_params, /* RES 1*/
#else
  &t4kb3_csi_params, /* RES 0*/
  &t4kb3_csi_params, /* RES 1*/
  &t4kb3_csi_params, /* RES 2*/
  &t4kb3_csi_params, /* RES 3*/
  &t4kb3_csi_params, /* RES 4*/
#endif
};

static struct sensor_lib_csi_params_array csi_params_array = {
  .csi2_params = &csi_params[0],
  .size = ARRAY_SIZE(csi_params),
};

static struct sensor_pix_fmt_info_t t4kb3_pix_fmt0_fourcc[] = {
	{ V4L2_PIX_FMT_SGRBG10 },//{ V4L2_PIX_FMT_SGBRG10 },
};

static struct sensor_pix_fmt_info_t t4kb3_pix_fmt1_fourcc[] = {
  { MSM_V4L2_PIX_FMT_META },
};

static sensor_stream_info_t t4kb3_stream_info[] = {
  {1, &t4kb3_cid_cfg[0], t4kb3_pix_fmt0_fourcc},
  {1, &t4kb3_cid_cfg[1], t4kb3_pix_fmt1_fourcc},
};

static sensor_stream_info_array_t t4kb3_stream_info_array = {
  .sensor_stream_info = t4kb3_stream_info,
  .size = ARRAY_SIZE(t4kb3_stream_info),
};

static struct msm_camera_i2c_reg_setting res_settings[] = {
#if FEATURE_CAPTURE_PREVIEW_RAW_IMAGE
  {
    .reg_setting = res1_reg_array,
    .size = ARRAY_SIZE(res1_reg_array),
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .data_type = MSM_CAMERA_I2C_BYTE_DATA,
    .delay = 10,
  },
  {
    .reg_setting = res1_reg_array,
    .size = ARRAY_SIZE(res1_reg_array),
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .data_type = MSM_CAMERA_I2C_BYTE_DATA,
    .delay = 10,
  },
#else
  {
    .reg_setting = res0_reg_array,
    .size = ARRAY_SIZE(res0_reg_array),
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .data_type = MSM_CAMERA_I2C_BYTE_DATA, 
    .delay = 10,
  },
  {
    .reg_setting = res1_reg_array,
    .size = ARRAY_SIZE(res1_reg_array),
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .data_type = MSM_CAMERA_I2C_BYTE_DATA,
    .delay = 10,
  },
  {
    .reg_setting = res2_reg_array,
    .size = ARRAY_SIZE(res2_reg_array),
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .data_type = MSM_CAMERA_I2C_BYTE_DATA,
    .delay = 10,
  },
  {
    .reg_setting = res3_reg_array,
    .size = ARRAY_SIZE(res3_reg_array),
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .data_type = MSM_CAMERA_I2C_BYTE_DATA,
    .delay = 10,
  },
  {
    .reg_setting = res4_reg_array,
    .size = ARRAY_SIZE(res4_reg_array),
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .data_type = MSM_CAMERA_I2C_BYTE_DATA,
    .delay = 10,
  },
#endif
};

static struct sensor_lib_reg_settings_array res_settings_array = {
  .reg_settings = res_settings,
  .size = ARRAY_SIZE(res_settings),
};

static struct sensor_crop_parms_t crop_params[] = {
#if FEATURE_CAPTURE_PREVIEW_RAW_IMAGE
  {0, 0, 0, 0}, /* RES 1 */
  {0, 0, 0, 0}, /* RES 1 */
#else
  {0, 0, 0, 0}, /* RES 0 */
  {0, 0, 0, 0}, /* RES 1 */
  {0, 0, 0, 0}, /* RES 2 */
  {0, 0, 0, 0}, /* RES 3 */
  {0, 0, 0, 0}, /* RES 4 */
#endif
};

static struct sensor_lib_crop_params_array crop_params_array = {
  .crop_params = crop_params,
  .size = ARRAY_SIZE(crop_params),
};

static struct sensor_lib_out_info_t sensor_out_info[] = {
#if FEATURE_CAPTURE_PREVIEW_RAW_IMAGE
  {/* 30 fps qtr size settings */ // H2V2_30
    .x_output = 2104,
    .y_output = 1560,
    .line_length_pclk = 4480,
    .frame_length_lines = 1584,
    .vt_pixel_clk = 213600000,
    .op_pixel_clk = 284800000,//
    .binning_factor = 1,
    .max_fps = 30.1,
    .min_fps = 7.5,
    .mode = SENSOR_DEFAULT_MODE,
  },
  {/* 30 fps qtr size settings */ // H2V2_30
    .x_output = 2104,
    .y_output = 1560,
    .line_length_pclk = 4480,
    .frame_length_lines = 1584,
    .vt_pixel_clk = 213600000,
    .op_pixel_clk = 284800000,//
    .binning_factor = 1,
    .max_fps = 30.1,
    .min_fps = 7.5,
    .mode = SENSOR_DEFAULT_MODE,
  },
#else
  {/* 30 fps full size settings */ //13M_30
	  .x_output = 4208, //4208,
	  .y_output = 3120, //3120,
	  .line_length_pclk = 4480, //4520,
	  .frame_length_lines = 3142, //3152,
	  .vt_pixel_clk = 422400000,
	  .op_pixel_clk = 422400000,//420268750,
	  .binning_factor = 1,
	  .max_fps = 30.01,//29.75,
	  .min_fps = 7.5,
	  .mode = SENSOR_DEFAULT_MODE,
	},
  {/* 30 fps qtr size settings */ // H2V2_30
    .x_output = 2104,
    .y_output = 1560,
    .line_length_pclk = 4480,
    .frame_length_lines = 1584,
    .vt_pixel_clk = 213600000,
    .op_pixel_clk = 284800000,//
    .binning_factor = 1,
    .max_fps = 30.1,
    .min_fps = 7.5,
    .mode = SENSOR_DEFAULT_MODE,
  },
  {/* 60 fps settings *///1920x1080
    .x_output = 1920,
    .y_output = 1080,
    .line_length_pclk = 4480,
    .frame_length_lines = 1304,
    .vt_pixel_clk = 352000000,
    .op_pixel_clk = 281600000,
    .binning_factor = 1,
    .max_fps = 60.25,
    .min_fps = 7.5,
    .mode = SENSOR_HFR_MODE,  
  },
  {/* 90 fps settings */ //TAB_NAME: 720p_90
    .x_output = 1280,
    .y_output = 720,
    .line_length_pclk = 4480,
    .frame_length_lines = 870,
    .vt_pixel_clk = 352000000,
    .op_pixel_clk = 211200000,
    .binning_factor = 1,
    .max_fps = 90.31,
    .min_fps = 7.5,
    .mode = SENSOR_HFR_MODE,
  },
  {/* 120 fps settings */
    .x_output = 640,
    .y_output = 480,
    .line_length_pclk = 4480,
    .frame_length_lines = 804,
    .vt_pixel_clk = 352000000,
    .op_pixel_clk = 281600000,
    .binning_factor = 1,
    .max_fps = 120.14,
    .min_fps = 7.5,
    .mode = SENSOR_HFR_MODE,
  },
#endif
};

static struct sensor_lib_out_info_array out_info_array = {
  .out_info = sensor_out_info,
  .size = ARRAY_SIZE(sensor_out_info),
};

static sensor_res_cfg_type_t t4kb3_res_cfg[] = {
  SENSOR_SET_STOP_STREAM,
  SENSOR_SET_NEW_RESOLUTION, /* set stream config */
  SENSOR_SET_CSIPHY_CFG,
  SENSOR_SET_CSID_CFG,
  SENSOR_LOAD_CHROMATIX, /* set chromatix prt */
  SENSOR_SEND_EVENT, /* send event */
  SENSOR_SET_START_STREAM,
};

static struct sensor_res_cfg_table_t t4kb3_res_table = {
  .res_cfg_type = t4kb3_res_cfg,
  .size = ARRAY_SIZE(t4kb3_res_cfg),
};

static struct sensor_lib_chromatix_t t4kb3_chromatix[] = {
#if FEATURE_CAPTURE_PREVIEW_RAW_IMAGE
  {
	  .common_chromatix = T4KB3_8939_LOAD_CHROMATIX(common),
	  .camera_preview_chromatix = T4KB3_8939_LOAD_CHROMATIX(preview), /* RES1 */
	  .camera_snapshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(preview), /* RES1 */
	  .camcorder_chromatix = T4KB3_8939_LOAD_CHROMATIX(default_video), /* RES1 */
	  .liveshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(liveshot), /* RES1 */
  },
  {
	  .common_chromatix = T4KB3_8939_LOAD_CHROMATIX(common),
	  .camera_preview_chromatix = T4KB3_8939_LOAD_CHROMATIX(preview), /* RES1 */
	  .camera_snapshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(preview), /* RES1 */
	  .camcorder_chromatix = T4KB3_8939_LOAD_CHROMATIX(default_video), /* RES1 */
	  .liveshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(liveshot), /* RES1 */
  },
#else
  {
    .common_chromatix = T4KB3_8939_LOAD_CHROMATIX(common),
    .camera_preview_chromatix = T4KB3_8939_LOAD_CHROMATIX(zsl), /* RES0 */
    .camera_snapshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(snapshot), /* RES0 */
    .camcorder_chromatix = T4KB3_8939_LOAD_CHROMATIX(snapshot), /* RES0 */
    .liveshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(liveshot), /* RES0 */
  },
  {
	  .common_chromatix = T4KB3_8939_LOAD_CHROMATIX(common),
	  .camera_preview_chromatix = T4KB3_8939_LOAD_CHROMATIX(preview), /* RES1 */
	  .camera_snapshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(preview), /* RES1 */
	  .camcorder_chromatix = T4KB3_8939_LOAD_CHROMATIX(default_video), /* RES1 */
	  .liveshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(liveshot), /* RES1 */
  },
  {
    .common_chromatix = T4KB3_8939_LOAD_CHROMATIX(common),
    .camera_preview_chromatix = T4KB3_8939_LOAD_CHROMATIX(hfr_60fps), /* RES2 */
    .camera_snapshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(hfr_60fps), /* RES2 */
    .camcorder_chromatix = T4KB3_8939_LOAD_CHROMATIX(hfr_60fps), /* RES2 */
    .liveshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(hfr_60fps), /* RES2 */
  },
  {
    .common_chromatix = T4KB3_8939_LOAD_CHROMATIX(common),
    .camera_preview_chromatix = T4KB3_8939_LOAD_CHROMATIX(hfr_90fps), /* RES3 */
    .camera_snapshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(hfr_90fps), /* RES3 */
    .camcorder_chromatix = T4KB3_8939_LOAD_CHROMATIX(hfr_90fps), /* RES3 */
    .liveshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(hfr_90fps), /* RES3 */
  },
  {
    .common_chromatix = T4KB3_8939_LOAD_CHROMATIX(common),
    .camera_preview_chromatix = T4KB3_8939_LOAD_CHROMATIX(hfr_120fps), /* RES4 */
    .camera_snapshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(hfr_120fps), /* RES4 */
    .camcorder_chromatix = T4KB3_8939_LOAD_CHROMATIX(hfr_120fps), /* RES4 */
    .liveshot_chromatix = T4KB3_8939_LOAD_CHROMATIX(hfr_120fps), /* RES4 */
  },
#endif
};

static struct sensor_lib_chromatix_array t4kb3_lib_chromatix_array = {
  .sensor_lib_chromatix = t4kb3_chromatix,
  .size = ARRAY_SIZE(t4kb3_chromatix),
};

/*===========================================================================
 * FUNCTION    - t4kb3_real_to_register_gain -
 *
 * DESCRIPTION:
 *==========================================================================*/
static uint16_t t4kb3_real_to_register_gain(float gain)
{
  uint16_t reg_gain;
#if 0
  if (gain < 1.0)
    gain = 1.0;

  reg_gain = (uint16_t)(gain * 406.0);
#else
  if (gain < 1.0)
    gain = 1.0;
  else if(gain > 15.5)
    gain = 15.5;
  reg_gain = (uint16_t)(128.0 * gain);//0x80
#endif

  return reg_gain;
}

/*===========================================================================
 * FUNCTION    - t4kb3_register_to_real_gain -
 *
 * DESCRIPTION:
 *==========================================================================*/
static float t4kb3_register_to_real_gain(uint16_t reg_gain)
{
  float real_gain;
#if 0
 // if (reg_gain > 232)
 //   reg_gain = 232;

  real_gain = (float)(reg_gain / 406.0);
#else
  if(reg_gain >= 2048)
  reg_gain = 0x7ff;
  
  real_gain = (float)(reg_gain / 128.0);
#endif
  return real_gain;
}

/*===========================================================================
 * FUNCTION    - t4kb3_calculate_exposure -
 *
 * DESCRIPTION:
 *==========================================================================*/
static int32_t t4kb3_calculate_exposure(float real_gain,
  uint16_t line_count, sensor_exposure_info_t *exp_info)
{
  if (!exp_info) {
    return -1;
  }
  exp_info->reg_gain = t4kb3_real_to_register_gain(real_gain);
  exp_info->sensor_real_gain = t4kb3_register_to_real_gain(exp_info->reg_gain);
  exp_info->digital_gain = real_gain / exp_info->sensor_real_gain;
  exp_info->line_count = line_count;
  exp_info->sensor_digital_gain = 0x1;
  return 0;
}

/*===========================================================================
 * FUNCTION    - t4kb3_fill_exposure_array -
 *
 * DESCRIPTION:
 *==========================================================================*/
static int32_t t4kb3_fill_exposure_array(uint16_t gain,
  uint32_t line, uint32_t fl_lines, int32_t luma_avg, uint32_t fgain,
  struct msm_camera_i2c_reg_setting *reg_setting)
{
  int32_t rc = 0;
  uint16_t reg_count = 0;
  uint16_t i = 0;

  if (!reg_setting) {
    return -1;
  }

  ALOGE("3a####gain = %d, line = %d, fl_lines = %d", gain, line, fl_lines);

  
  current_gain = gain;
  global_gain = gain; //406 0x196    //128  0x80
    
  ALOGE("global_gain = %d, prev_gain = %d, current_gain = %d", prev_gain, prev_gain, current_gain);
  
#if 1
  for (i = 0; i < sensor_lib_ptr.groupon_settings->size; i++) {
    reg_setting->reg_setting[reg_count].reg_addr =
      sensor_lib_ptr.groupon_settings->reg_setting[i].reg_addr;
    reg_setting->reg_setting[reg_count].reg_data =
      sensor_lib_ptr.groupon_settings->reg_setting[i].reg_data;
    reg_count = reg_count + 1;
  }
#endif
  
#if 0
	for (i = 0; i < sensor_lib_ptr.groupoff_settings->size; i++) {
	  reg_setting->reg_setting[reg_count].reg_addr =
		sensor_lib_ptr.groupoff_settings->reg_setting[i].reg_addr;
	  reg_setting->reg_setting[reg_count].reg_data =
		sensor_lib_ptr.groupoff_settings->reg_setting[i].reg_data;
	  reg_count = reg_count + 1;
	}
#endif

/*
	global_gain = 1280;
	fl_lines = 3136;
	line = 3136;
*/

  reg_setting->reg_setting[reg_count].reg_addr =
    sensor_lib_ptr.output_reg_addr->frame_length_lines;
  reg_setting->reg_setting[reg_count].reg_data = (fl_lines & 0xFF00) >> 8;
  reg_count++;

  reg_setting->reg_setting[reg_count].reg_addr =
    sensor_lib_ptr.output_reg_addr->frame_length_lines + 1;
  reg_setting->reg_setting[reg_count].reg_data = (fl_lines & 0xFF);
  reg_count++;

  reg_setting->reg_setting[reg_count].reg_addr =
    sensor_lib_ptr.exp_gain_info->coarse_int_time_addr;
  reg_setting->reg_setting[reg_count].reg_data = (line & 0xFF00) >> 8;
  reg_count++;

  reg_setting->reg_setting[reg_count].reg_addr =
    sensor_lib_ptr.exp_gain_info->coarse_int_time_addr + 1;
  reg_setting->reg_setting[reg_count].reg_data = (line & 0xFF);
  reg_count++;

  reg_setting->reg_setting[reg_count].reg_addr =
    sensor_lib_ptr.exp_gain_info->global_gain_addr;
  reg_setting->reg_setting[reg_count].reg_data = (global_gain & 0xFF00) >> 8;
  reg_count++;
  
  reg_setting->reg_setting[reg_count].reg_addr =
    sensor_lib_ptr.exp_gain_info->global_gain_addr + 1;
  reg_setting->reg_setting[reg_count].reg_data = (global_gain & 0xFF);
  reg_count++;
  
#if 1
  for (i = 0; i < sensor_lib_ptr.groupoff_settings->size; i++) {
    reg_setting->reg_setting[reg_count].reg_addr =
      sensor_lib_ptr.groupoff_settings->reg_setting[i].reg_addr;
    reg_setting->reg_setting[reg_count].reg_data =
      sensor_lib_ptr.groupoff_settings->reg_setting[i].reg_data;
    reg_count = reg_count + 1;
  }
#endif

  reg_setting->size = reg_count;
  reg_setting->addr_type = MSM_CAMERA_I2C_WORD_ADDR;
  reg_setting->data_type = MSM_CAMERA_I2C_BYTE_DATA;
  reg_setting->delay = 0;

  prev_gain = current_gain;
  
  return rc;
}

static sensor_exposure_table_t t4kb3_expsoure_tbl = {
  .sensor_calculate_exposure = t4kb3_calculate_exposure,
  .sensor_fill_exposure_array = t4kb3_fill_exposure_array,
};

static sensor_lib_t sensor_lib_ptr = {
  /* sensor slave info */
  .sensor_slave_info = &sensor_slave_info,
  /* sensor init params */
  .sensor_init_params = &sensor_init_params,
  /* sensor actuator name */
  .actuator_name = "rohm_bu64243gwz_t4kb3",
  /* sensor eeprom name */
  .eeprom_name = "toshiba_st1382",
  /* sensor output settings */
  .sensor_output = &sensor_output,
  /* sensor output register address */
  .output_reg_addr = &output_reg_addr,
  /* sensor exposure gain register address */
  .exp_gain_info = &exp_gain_info,
  /* sensor aec info */
  .aec_info = &aec_info,
  /* sensor snapshot exposure wait frames info */
  .snapshot_exp_wait_frames = 1,
  /* number of frames to skip after start stream */
  .sensor_num_frame_skip = 1,
  /* number of frames to skip after start HDR stream */
  .sensor_num_HDR_frame_skip = 2,
  /* sensor pipeline immediate delay */
  .sensor_max_pipeline_frame_delay = 1,
  /* sensor exposure table size */
  .exposure_table_size = 7,
  /* sensor lens info */
  .default_lens_info = &default_lens_info,
  /* csi lane params */
  .csi_lane_params = &csi_lane_params,
  /* csi cid params */
  .csi_cid_params = t4kb3_cid_cfg,
  /* csi csid params array size */
  .csi_cid_params_size = ARRAY_SIZE(t4kb3_cid_cfg),
  /* init settings */
  .init_settings_array = &init_settings_array,
  /* start settings */
  .start_settings = &start_settings,
  /* stop settings */
  .stop_settings = &stop_settings,
  /* group on settings */
  .groupon_settings = &groupon_settings,
  /* group off settings */
  .groupoff_settings = &groupoff_settings,
  /* resolution cfg table */
  .sensor_res_cfg_table = &t4kb3_res_table,
  /* res settings */
  .res_settings_array = &res_settings_array,
  /* out info array */
  .out_info_array = &out_info_array,
  /* crop params array */
  .crop_params_array = &crop_params_array,
  /* csi params array */
  .csi_params_array = &csi_params_array,
  /* sensor port info array */
  .sensor_stream_info_array = &t4kb3_stream_info_array,
  /* exposure funtion table */
  .exposure_func_table = &t4kb3_expsoure_tbl,
  /* chromatix array */
  .chromatix_array = &t4kb3_lib_chromatix_array,
  /* sensor pipeline immediate delay */
  .sensor_max_immediate_frame_delay = 2,
};

/*===========================================================================
 * FUNCTION    - t4kb3_open_lib -
 *
 * DESCRIPTION:
 *==========================================================================*/
void *t4kb3_open_lib(void)
{
  return &sensor_lib_ptr;
}

