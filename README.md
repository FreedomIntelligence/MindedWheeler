# EmbodyAICar
Embody_AI with car as Demo

<center>
    <img src="https://github.com/FreedomIntelligence/EmbodyAICar/blob/main/assets/embodyaicar.png" width="450" style="display: inline-block; vertical-align: top;"/>
</center>




<p align="center">
   🌐 <a href="" target="_blank">Website</a> • 🤗 <a href="" target="_blank">Model</a>  
</p>

## 🌈 Update
* **[2024.02.23]** 🎉🎉🎉 EmbodyAICar is published！🎉🎉🎉

## 🤖 Model Training Data

    ```
    User：快速向左转
    RobotAI: (1.0, -0.3)    
    ```

- The two float are in range [-1,1]
- The first float is speed, the second is direction(negative means left, positive means right).

## 🤖 Communication Protocol

- 0x33 0x33 0x03 {} sum  (See detail in [code](https://github.com/FreedomIntelligence/EmbodyAICar/blob/main/qwen.cpp#L151))
    

## ℹ️ Usage
1. DownLoad [Model]() and Follow [Qwen.cpp](https://github.com/QwenLM/qwen.cpp.git) get model.bin and qwen.tiktoken.
2. Install package serial.tar.gz
   ```
    cd serial
    cmake .. & make & sudo make install
    ```
3. Compile the project using CMake:
    ```
    cmake -B build
    cmake --build build -j --config Release
    ```
4. Now you may chat and control your AI car with the quantized RobotAI model by running:
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
@misc{embodyAiCar,
  title={EmbodyAICar: Embody_AI with car as Demo},
  author={Xidong Wang*, Yuan Shen*},
  year = {2024},
  publisher = {GitHub},
  journal = {GitHub repository},
  howpublished = {\url{https://github.com/FreedomIntelligence/EmbodyAICar}},
}
```


## 🤖 Acknowledgement 
- We thank [Qwen.cpp](https://github.com/QwenLM/qwen.cpp.git) and [llama.cpp](https://github.com/ggerganov/llama.cpp) for their excellent work.
