/*============================================================================

  Copyright (c) 2014 Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

============================================================================*/
/*#ifndef __T4KB3_8939_LIB_H__
#define __T4KB3_8939_LIB_H__
*/
#include "sensor_lib.h"

#define SENSOR_MODEL_NO_SKUF_T4KB3_8939 "t4kb3"
#define SKUF_T4KB3_8939_LOAD_CHROMATIX(n) \
  "libchromatix_"SENSOR_MODEL_NO_SKUF_T4KB3_8939"_"#n".so"

static struct msm_camera_i2c_reg_array init_reg_array0[] = {
  {0x0103, 0x01}, //reset
};

static struct msm_camera_i2c_reg_array init_reg_array1[] = {

{0x0000,0x1C}, //model_id;
{0x0001,0x50}, //model_id;
{0x0101,0x00}, //image_orientation;
{0x0103,0x00}, //software_reset;
{0x0104,0x00}, //grouped_parameter_hold;
{0x0105,0x00}, //mask_corrupted_frame;
{0x0110,0x40}, //CSI_channel_identifier;
{0x0111,0x02}, //CSI_signaling_mode;
{0x0112,0x0A}, //CSI_data_format[15:8];
{0x0115,0x30}, //CSI_10-to-8_DT;
{0x4136,0x18}, //EXTCLK_FREQ_MHZ;
{0x4137,0x00}, //EXTCLK_FREQ_MHZ;
{0x0138,0x01}, //Temp_Sensor_control;
{0x013A,0x60}, //Temp_Sensor_OUT;
{0x0204,0x00}, //analogue_gain_code_global[15:8];
{0x0205,0x7A}, //analogue_gain_code_global[7:0];
{0x020E,0x01}, //digital_gain_greenR[11:8];
{0x020F,0x00}, //digital_gain_greenR[7:0];
{0x0210,0x01}, //digital_gain_red[11:8];
{0x0211,0x00}, //digital_gain_red[7:0];
{0x0212,0x01}, //digital_gain_blue[11:8];
{0x0213,0x00}, //digital_gain_blue[7:0];
{0x0214,0x01}, //digital_gain_greenB[11:8];
{0x0215,0x00}, //digital_gain_greenB[7:0];
{0x4224,0x00}, //HDR_direct_short_integration_time[15:8];
{0x4225,0x19}, //HDR_direct_short_integration_time[7:0];
{0x0233,0x01}, //Ag_Addr_Sel;//[0] 1:AG_MAG enable T4KA3/B3  [0]0 AG_MAG disable(using ANA_GA_CODE_GL) T4K82
//{0x0233,0x00},
{0x0234,0x00}, //Ag_Mag[10:8];
{0x0235,0x80}, //Ag_Mag[7:0];
{0x0305,0x01}, //pre_pll_clk_div;
{0x0306,0x00}, //pll_multiplier[8];
{0x0307,0x58}, //pll_multiplier[7:0];
{0x0345,0x00}, //x_addr_start[7:0];
{0x0349,0x6F}, //x_addr_end[7:0];
{0x0408,0x00}, //digital_crop_x_offset[15:8];
{0x040A,0x00}, //digital_crop_y_offset[15:8];
{0x040B,0x00}, //digital_crop_y_offset[7:0];
{0x0601,0x00}, //test_pattern_mode;
{0x0602,0x02}, //test_data_red[9:8];
{0x0603,0xC0}, //test_data_red[7:0];
{0x0604,0x02}, //test_data_greenR[9:8];
{0x0605,0xC0}, //test_data_greenR[7:0];
{0x0606,0x02}, //test_data_blue[9:8];
{0x0607,0xC0}, //test_data_blue[7:0];
{0x0608,0x02}, //test_data_greenB[9:8];
{0x0609,0xC0}, //test_data_greenB[7:0];
{0x0800,0x88}, //TCLK_POST[7:3];
{0x0801,0x38}, //THS_PREPARE[7:3];
{0x0802,0x78}, //THS_ZERO[7:3];
{0x0803,0x48}, //THS_TRAIL[7:3];
{0x0804,0x48}, //TCLK_TRAIL[7:3];
{0x0805,0x40}, //TCLK_PREPARE[7:3];
{0x0806,0x00}, //TCLK_ZERO[7:3];
{0x0807,0x48}, //TLPX[4:0];
{0x0808,0x01}, //DPHY_CTRL[1:0];
{0x0822,0x00}, //REQUESTED_LINK_BIT_RATE_MBPS [15:8];
{0x0823,0x00}, //REQUESTED_LINK_BIT_RATE_MBPS [7:0];
{0x0900,0x01}, //Binning_mode;
{0x4B05,0x01}, //-/-/-/-/-/-/-/MAP_DEF_EN;
{0x4B06,0x01}, //-/-/-/-/-/-/-/HADPC_W_EN;
{0x4B07,0x01}, //-/-/-/-/-/-/-/HADPC_B_EN;
{0x4C12,0x01}, //FLASH_ADJ[7:0];
{0x4C14,0x00}, //Flash_strobe_start_point [15:8];
{0x4C15,0x01}, //Flash_strobe_start_point [7:0];
{0x4C16,0x00}, //tFlash_strobe_delay_rs_ctrl [15:8];
{0x4C17,0x20}, //tFlash_strobe_delay_rs_ctrl [7:0];
{0x4C18,0x00}, //tFlash_strobe_width_high_rs_ctrl [15:8];
{0x4C19,0x40}, //tFlash_strobe_width_high_rs_ctrl [7:0];
{0x4C1A,0x00}, //-/-/-/-/FLASH_MODE[3]/FLASH_MODE[2]/FLASH_MODE[1]/FLASH_MODE[0];
{0x4C1B,0x00}, //-/-/-/-/-/-/-/Flash_Trigger_‚’‚“;
{0x4C1C,0x00}, //-/-/-/-/-/-/Flash_status [1:0];
{0x4C26,0x00}, //tFlash_strobe_width2_high_rs_ctrl [15:8];
{0x4C27,0x00}, //tFlash_strobe_width2_high_rs_ctrl [7:0];
{0x4C28,0x00}, //tFlash_strobe_width_low_rs_ctrl [15:8];
{0x4C29,0x00}, //tFlash_strobe_width_low_rs_ctrl [7:0];
{0x4C2A,0x00}, //tFlash_strobe_count_rs_ctrl [7:0];
{0x306D,0x0A}, //Reserved;
{0x3071,0xFA}, //Reserved;
{0x3094,0x01}, //-/-/-/-/-/-/-/LP_MODE;
{0x3098,0xB6}, //Reserved;
{0x315B,0x34}, //Reserved;
{0x315C,0x34}, //Reserved;
{0x315D,0x38}, //Reserved;
{0x3169,0x05}, //Reserved;
{0x3170,0x77}, //Reserved;
{0x3171,0x77}, //Reserved;
{0x31A1,0xB2}, //Reserved;
{0x31A2,0xAA}, //Reserved;
{0x31A3,0x90}, //Reserved;
{0x31A4,0x9A}, //Reserved;
{0x31A5,0xF0}, //Reserved;
{0x31A6,0xEE}, //Reserved;
{0x31A8,0xF4}, //Reserved;
{0x3216,0x58}, //Reserved;
{0x3217,0x58}, //Reserved;
{0x3218,0x58}, //Reserved;
{0x321A,0x68}, //Reserved;
{0x321B,0x60}, //Reserved;
{0x3238,0x20}, //Reserved;
{0x323B,0x40}, //Reserved;
{0x3243,0x03}, //-/-/-/-/-/-/LSC_EN/SHD_GRID_EN;
{0x3244,0x09}, //Reserved;
{0x3245,0x01}, //Reserved;
{0x3247,0x03}, //Reserved;
{0x3250,0x00}, //PWB_GRG[7:0];
{0x3251,0x00}, //PWB_GBG[7:0];
{0x3380,0x01}, //Reserved;
{0x339E,0x07}, //Reserved;
{0x3386,0x01}, //Reserved;
};

static struct msm_camera_i2c_reg_array res0_reg_array[] = {
  //13M_30
{0x0100,0x00}, //mode_select;
{0x0306,0x00}, //pll_multiplier[8];
{0x0307,0x58}, //pll_multiplier[7:0];
{0x0113,0x0A}, //CSI_data_format7:0];
{0x0114,0x03}, //CSI_lane_mode;
{0x0202,0x0C}, //coarse_integration_time[15:8];
{0x0203,0x46}, //coarse_integration_time[7:0];
{0x4220,0x00}, //-/-/-/HDR_SHORT_EXPOSURE_SW/HDR_MODE_SW/-/-/HDR_SW;
{0x4222,0x04}, //HDR_short_exposure_ratio;
{0x0301,0x01}, //vt_pix_clk_div;
{0x0303,0x0A}, //vt_sys_clk_div;
{0x030B,0x01}, //op_sys_clk_div;
{0x0340,0x0C}, //vt_frame_length_lines[15:8];
{0x0341,0x46}, //vt_frame_length_lines[7:0];
{0x0342,0x11}, //vt_line_length_pck[15:8];
{0x0343,0x80}, //vt_line_length_pck[7:0];
{0x0344,0x00}, //x_addr_start[15:8];
{0x0346,0x00}, //y_addr_start[15:8];
{0x0347,0x00}, //y_addr_start[7:0];
{0x0348,0x10}, //x_addr_end[15:8];
{0x034A,0x0C}, //y_addr_end[15:8];
{0x034B,0x2F}, //y_addr_end[7:0];
{0x034C,0x10}, //x_output_size[15:8];
{0x034D,0x70}, //x_output_size[7:0];
{0x034E,0x0C}, //y_output_size[15:8];
{0x034F,0x30}, //y_output_size[7:0];
{0x0409,0x00}, //digital_crop_x_offset[7:0];
{0x040C,0x10}, //digital_crop_image_width[15:8];
{0x040D,0x70}, //digital_crop_image_width[7:0];
{0x040E,0x0C}, //digital_crop_image_height[15:8];
{0x040F,0x30}, //digital_crop_image_height[7:0];
{0x0820,0x10}, //REQUESTED_LINK_BIT_RATE_MBPS [31:24];
{0x0821,0x80}, //REQUESTED_LINK_BIT_RATE_MBPS [23:16];
{0x0901,0x11}, //Binning_type_H[7:4]/Binning_type_V[3:0];
//{0x0100,0x01}, //mode_select;
};

static struct msm_camera_i2c_reg_array res1_reg_array[] = {
  //TAB_NAME: H2V2_30 preview
{0x0100,0x00}, //mode_select;
{0x0306,0x00}, //pll_multiplier[8];
{0x0307,0x59}, //pll_multiplier[7:0];
{0x0113,0x0A}, //CSI_data_format7:0];
{0x0114,0x03}, //CSI_lane_mode;
{0x0202,0x06}, //coarse_integration_time[15:8];
{0x0203,0x2A}, //coarse_integration_time[7:0];
{0x4220,0x00}, //-/-/-/HDR_SHORT_EXPOSURE_SW/HDR_MODE_SW/-/-/HDR_SW;
{0x4222,0x04}, //HDR_short_exposure_ratio;
{0x0301,0x02}, //vt_pix_clk_div;
{0x0303,0x0A}, //vt_sys_clk_div;
{0x030B,0x02}, //op_sys_clk_div;
{0x0340,0x06}, //vt_frame_length_lines[15:8];
{0x0341,0x30}, //vt_frame_length_lines[7:0];
{0x0342,0x11}, //vt_line_length_pck[15:8];
{0x0343,0x80}, //vt_line_length_pck[7:0];
{0x0344,0x00}, //x_addr_start[15:8];
{0x0346,0x00}, //y_addr_start[15:8];
{0x0347,0x00}, //y_addr_start[7:0];
{0x0348,0x10}, //x_addr_end[15:8];
{0x034A,0x0C}, //y_addr_end[15:8];
{0x034B,0x2F}, //y_addr_end[7:0];
{0x034C,0x08}, //x_output_size[15:8];
{0x034D,0x38}, //x_output_size[7:0];
{0x034E,0x06}, //y_output_size[15:8];
{0x034F,0x18}, //y_output_size[7:0];
{0x0409,0x00}, //digital_crop_x_offset[7:0];
{0x040C,0x08}, //digital_crop_image_width[15:8];
{0x040D,0x38}, //digital_crop_image_width[7:0];
{0x040E,0x06}, //digital_crop_image_height[15:8];
{0x040F,0x18}, //digital_crop_image_height[7:0];
{0x0820,0x08}, //REQUESTED_LINK_BIT_RATE_MBPS [31:24];
{0x0821,0x40}, //REQUESTED_LINK_BIT_RATE_MBPS [23:16];
{0x0901,0x22}, //Binning_type_H[7:4]/Binning_type_V[3:0];
//{0x0100,0x01}, //mode_select;
};

static struct msm_camera_i2c_reg_array res2_reg_array[] = {
//TAB_NAME: 1080p_60

{0x0100,0x00}, //mode_select;
{0x0113,0x0A}, //CSI_data_format7:0];
{0x0114,0x03}, //CSI_lane_mode;
{0x0202,0x05}, //coarse_integration_time[15:8];
{0x0203,0x12}, //coarse_integration_time[7:0];
{0x4220,0x00}, //-/-/-/HDR_SHORT_EXPOSURE_SW/HDR_MODE_SW/-/-/HDR_SW;
{0x4222,0x04}, //HDR_short_exposure_ratio;
{0x0301,0x01}, //vt_pix_clk_div;
{0x0303,0x0C}, //vt_sys_clk_div;
{0x030B,0x02}, //op_sys_clk_div;
{0x0340,0x05}, //vt_frame_length_lines[15:8];
{0x0341,0x18}, //vt_frame_length_lines[7:0];
{0x0342,0x11}, //vt_line_length_pck[15:8];
{0x0343,0x80}, //vt_line_length_pck[7:0];
{0x0344,0x00}, //x_addr_start[15:8];
{0x0346,0x01}, //y_addr_start[15:8];
{0x0347,0xE0}, //y_addr_start[7:0];
{0x0348,0x10}, //x_addr_end[15:8];
{0x034A,0x0A}, //y_addr_end[15:8];
{0x034B,0x4F}, //y_addr_end[7:0];
{0x034C,0x07}, //x_output_size[15:8];
{0x034D,0x80}, //x_output_size[7:0];
{0x034E,0x04}, //y_output_size[15:8];
{0x034F,0x38}, //y_output_size[7:0];
{0x0409,0x5C}, //digital_crop_x_offset[7:0];
{0x040C,0x07}, //digital_crop_image_width[15:8];
{0x040D,0x80}, //digital_crop_image_width[7:0];
{0x040E,0x04}, //digital_crop_image_height[15:8];
{0x040F,0x38}, //digital_crop_image_height[7:0];
{0x0820,0x08}, //REQUESTED_LINK_BIT_RATE_MBPS [31:24];
{0x0821,0x40}, //REQUESTED_LINK_BIT_RATE_MBPS [23:16];
{0x0901,0x22}, //Binning_type_H[7:4]/Binning_type_V[3:0];
//{0x0100,0x01}, //mode_select;
};

static struct msm_camera_i2c_reg_array res3_reg_array[] = {
	//TAB_NAME: 720p_90
{0x0100,0x00}, //mode_select;
{0x0113,0x0A}, //CSI_data_format7:0];
{0x0114,0x03}, //CSI_lane_mode;
{0x0202,0x03}, //coarse_integration_time[15:8];
{0x0203,0x0A}, //coarse_integration_time[7:0];
{0x4220,0x00}, //-/-/-/HDR_SHORT_EXPOSURE_SW/HDR_MODE_SW/-/-/HDR_SW;
{0x4222,0x04}, //HDR_short_exposure_ratio;
{0x0301,0x02}, //vt_pix_clk_div;
{0x0303,0x06}, //vt_sys_clk_div;
{0x030B,0x03}, //op_sys_clk_div;
{0x0340,0x03}, //vt_frame_length_lines[15:8];
{0x0341,0x66}, //vt_frame_length_lines[7:0];
{0x0342,0x11}, //vt_line_length_pck[15:8];
{0x0343,0x80}, //vt_line_length_pck[7:0];
{0x0344,0x00}, //x_addr_start[15:8];
{0x0346,0x01}, //y_addr_start[15:8];
{0x0347,0xE0}, //y_addr_start[7:0];
{0x0348,0x10}, //x_addr_end[15:8];
{0x034A,0x0A}, //y_addr_end[15:8];
{0x034B,0x4F}, //y_addr_end[7:0];
{0x034C,0x05}, //x_output_size[15:8];
{0x034D,0x00}, //x_output_size[7:0];
{0x034E,0x02}, //y_output_size[15:8];
{0x034F,0xD0}, //y_output_size[7:0];
{0x0409,0x3C}, //digital_crop_x_offset[7:0];
{0x040C,0x05}, //digital_crop_image_width[15:8];
{0x040D,0x00}, //digital_crop_image_width[7:0];
{0x040E,0x02}, //digital_crop_image_height[15:8];
{0x040F,0xD0}, //digital_crop_image_height[7:0];
{0x0820,0x05}, //REQUESTED_LINK_BIT_RATE_MBPS [31:24];
{0x0821,0x80}, //REQUESTED_LINK_BIT_RATE_MBPS [23:16];
{0x0901,0x33}, //Binning_type_H[7:4]/Binning_type_V[3:0];
//{0x0100,0x01}, //mode_select;
};

static struct msm_camera_i2c_reg_array res4_reg_array[] = {
{0x0100,0x00}, //mode_select;
{0x0112,0x0a}, //Reserved;
{0x0113,0x0A}, //CSI_data_format7:0];
{0x0114,0x03}, //CSI_lane_mode;
{0x4136,0x18}, //CSI_data_format7:0];
{0x4137,0x00}, //CSI_lane_mode;
{0x0820,0x0d}, //REQUESTED_LINK_BIT_RATE_MBPS [31:24];
{0x0821,0x50}, //REQUESTED_LINK_BIT_RATE_MBPS [23:16];
{0x0822,0x00}, //Reserved;
{0x0823,0x00}, //Reserved;
{0x0301,0x02}, //vt_pix_clk_div;
{0x0303,0x04}, //vt_sys_clk_div;
{0x0305,0x01}, //Reserved;
{0x0306,0x00}, //Reserved;
{0x0307,0x47}, //Reserved;
{0x030B,0x00}, //op_sys_clk_div;
{0x034C,0x02}, //x_output_size[15:8];
{0x034D,0x80}, //x_output_size[7:0];
{0x034E,0x01}, //y_output_size[15:8];
{0x034F,0xe0}, //y_output_size[7:0];
{0x0340,0x03}, //vt_frame_length_lines[15:8];
{0x0341,0x24}, //vt_frame_length_lines[7:0];
{0x0342,0x11}, //vt_line_length_pck[15:8];
{0x0343,0x80}, //vt_line_length_pck[7:0];
{0x0344,0x00}, //x_addr_start[15:8];0x3243,0x00
{0x0345,0x00}, //x_addr_start[15:8];
{0x0346,0x00}, //y_addr_start[15:8];
{0x0347,0x00}, //y_addr_start[7:0];
{0x0348,0x10}, //x_addr_end[15:8];
{0x0349,0x6f}, //x_addr_end[15:8];
{0x034A,0x0c}, //y_addr_end[15:8];
{0x034B,0x2f}, //y_addr_end[7:0];
{0x0408,0x00},
{0x0409,0xce}, //digital_crop_x_offset[7:0];
{0x040a,0x00},
{0x040b,0x96},
{0x040C,0x02}, //digital_crop_image_width[15:8];
{0x040D,0x80}, //digital_crop_image_width[7:0];
{0x040e,0x01}, //digital_crop_image_width[15:8];
{0x040f,0xe0}, //digital_crop_image_width[7:0];
{0x0900,0x01}, //Binning_type_H[7:4]/Binning_type_V[3:0];
{0x0901,0x44}, //Binning_type_H[7:4]/Binning_type_V[3:0];
{0x4220,0x00}, //-/-/-/HDR_SHORT_EXPOSURE_SW/HDR_MODE_SW/-/-/HDR_SW;
{0x4222,0x01}, //HDR_short_exposure_ratio;
{0x3245,0x01}, //-/-/-/HDR_SHORT_EXPOSURE_SW/HDR_MODE_SW/-/-/HDR_SW;
{0x3380,0x01}, //HDR_short_exposure_ratio;
//{0x0100,0x01}, //mode_select;
//{0x0100,0x01}, //mode_select;
};
