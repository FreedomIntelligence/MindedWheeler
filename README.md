# MindedWheeler
Embody_AI with car as Demo

<center>
    <img src="https://github.com/FreedomIntelligence/MindedWheeler/blob/main/assets/MindedWheeler.png" width="450" style="display: inline-block; vertical-align: top;"/>
</center>




<p align="center">
   🌐 <a href="" target="_blank">Website</a> • 🤗 <a href="https://huggingface.co/FreedomIntelligence/MindedWheeler" target="_blank">Model</a>  
</p>

## 🌈 Update
* **[2024.02.23]** 🎉🎉🎉 MindedWheeler is published！🎉🎉🎉

## 🤖 Model Training Data

```
User：快速向左转
RobotAI: (1.0, -0.3)    
...
```

- The two float are in range [-1,1]
- The first float is speed, the second is direction (negative means left, positive means right).

## 🤖 Communication Protocol

- 0x02, 0x02, 0x01, 8, data_buf;  (See detail in [code](https://github.com/FreedomIntelligence/MindedWheeler/blob/main/qwen.cpp#L151))
    

## ℹ️ Usage
1. DownLoad 🤗 [Model](https://huggingface.co/FreedomIntelligence/MindedWheeler) get model.bin.
   ```
   cd MindedWheeler
   git submodule update --init --recursive
   python qwen_cpp/convert.py -i {Model_Path} -t {type} -o robot1_8b-ggml.bin
   ```
   You are free to try any of the below quantization types by specifying -t <type>:

    - q4_0: 4-bit integer quantization with fp16 scales.
    - q4_1: 4-bit integer quantization with fp16 scales and minimum values.
    - q5_0: 5-bit integer quantization with fp16 scales.
    - q5_1: 5-bit integer quantization with fp16 scales and minimum values.
    - q8_0: 8-bit integer quantization with fp16 scales.
    - f16: half precision floating point weights without quantization.
    - f32: single precision floating point weights without quantization.

2. Install package serial.tar.gz
   ```
    tar zxvf serial.tar.gz
    cd serial
    cmake .. && make && sudo make install
    ```
3. Compile the project using CMake:
    ```
    cmake -B build
    cmake --build build -j --config Release
    ```
4. Now you may chat and control your AI car with the quantized RobotAI model by running:
   - qwen.tiktoken is in the model directory
   ```
   ./build/bin/main -m robot1_8b-ggml.bin --tiktoken qwen.tiktoken -p 请快速向前
   ```

   To run the model in interactive mode, add the -i flag. For example:

     ```
     ./build/bin/main -m robot1_8b-ggml.bin --tiktoken qwen.tiktoken -i
     ```

   In interactive mode, your chat history will serve as the context for the next-round conversation.

## 🥸 To do list

- Continue to create data and train a robust model
- Add ASR and TTS
- ...


## ✨ Citation
Please use the following citation if you intend to use our dataset for training or evaluation:



```
@misc{MindedWheeler,
  title={MindedWheeler: Embody_AI with car as Demo},
  author={Xidong Wang*, Yuan Shen*},
  year = {2024},
  publisher = {GitHub},
  journal = {GitHub repository},
  howpublished = {\url{https://github.com/FreedomIntelligence/MindedWheeler}},
}
```


## 🤖 Acknowledgement 
- We thank [Qwen.cpp](https://github.com/QwenLM/qwen.cpp.git) and [llama.cpp](https://github.com/ggerganov/llama.cpp) for their excellent work.
