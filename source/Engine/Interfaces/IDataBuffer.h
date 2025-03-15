//
// Created by Shaun on 15/03/2025.
//

#ifndef IDATABUFFER_H
#define IDATABUFFER_H

class IDataBuffer {
  public:
    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual ~IDataBuffer() = default;
    
    private:
      unsigned int m_dataBuffer;
};
  

#endif //IDATABUFFER_H
