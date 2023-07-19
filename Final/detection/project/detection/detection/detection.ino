#include <Camera.h>
#include "person_detect_model.h"

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

const int offset_x = 32;
const int offset_y = 12;
const int width    = 160;
const int height   = 120;
const int target_w = 96;
const int target_h = 96;
const int pixfmt   = CAM_IMAGE_PIX_FMT_YUV422;

constexpr int kTensorArenaSize = 100000;
uint8_t tensor_arena[kTensorArenaSize];

void CamCB(CamImage img) {
  static uint32_t last_mills = 0;

  if (!img.isAvailable()) {
    Serial.println("图像不可用");
    return;
  }

  uint16_t* buf = (uint16_t*)img.getImgBuff();   
  int n = 0; 
  for (int y = offset_y; y < offset_y + target_h; ++y) {
    for (int x = offset_x; x < offset_x + target_w; ++x) {
      uint16_t value = buf[y*width + x];
      uint16_t y_h = (value & 0xf000) >> 8;
      uint16_t y_l = (value & 0x00f0) >> 4;
      value = (y_h | y_l);      
      input->data.f[n++] = (float)(value)/255.0;
    }
  }

  Serial.println("推断中");

  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    Serial.println("调用tflite失败");
    return;
  }

  bool result = false;
  int8_t person_score = output->data.int8[1];
  int8_t no_person_score = output->data.int8[0];
  Serial.print("有人脸得分： " + String(person_score) + ", ");
  Serial.println("无人脸得分 " + String(no_person_score));
  if ((person_score > no_person_score) && (person_score > 40)) {
    digitalWrite(LED3, HIGH);
    result = true;
  } else {
    digitalWrite(LED3, LOW);
  }

  disp_image(buf, offset_x, offset_y, target_w, target_h, result);
  
  uint32_t current_mills = millis();
  uint32_t duration = current_mills - last_mills;
  Serial.println("间隔时间" + String(duration));
  last_mills = current_mills; 
}


void setup() {
  Serial.begin(115200);
  setup_display();

  tflite::InitializeTarget();
  memset(tensor_arena, 0, kTensorArenaSize*sizeof(uint8_t));
  
  //初始化模型
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  //将模型映射到数据
  model = tflite::GetModel(model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("提供的模型版本号：" 
                  + String(model->version()) + "不等于"
                  + "支持的版本号"
                  + String(TFLITE_SCHEMA_VERSION));
    return;
  } else {
    Serial.println("模型版本" + String(model->version()));
  }

  static tflite::AllOpsResolver resolver;
  //生成解释器
  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;
  //分配内存
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    Serial.println("AllocateTensors()失败");
    return;
  } else {
    Serial.println("AllocateTensor()成功");
  }

  size_t used_size = interpreter->arena_used_bytes();
  Serial.println("使用字节数：" + String(used_size));
  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("模型输入：");
  Serial.println("dims->size: " + String(input->dims->size));
  for (int n = 0; n < input->dims->size; ++n) {
    Serial.println("dims->data[" + String(n) + "]: " + String(input->dims->data[n]));
  }
  Serial.println("模型输出");
  Serial.println("dims->size: " + String(output->dims->size));
  for (int n = 0; n < output->dims->size; ++n) {
    Serial.println("dims->data[" + String(n) + "]: " + String(output->dims->data[n]));
  }

  Serial.println("tensorflow设置完成");
  digitalWrite(LED0, HIGH); 
  
  CamErr err = theCamera.begin(1, CAM_VIDEO_FPS_15, width, height, pixfmt);
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("照相机设置失败" + String(err));
    return;
  }
  err = theCamera.startStreaming(true, CamCB);
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("视频流获取失败" + String(err));
    return;
  }
}

void loop() {
}