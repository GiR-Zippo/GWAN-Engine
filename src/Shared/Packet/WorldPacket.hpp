#ifndef __WORLDPACKET_H
#define __WORLDPACKET_H

#include "Define.hpp"
#include "OpcodeDefines.hpp"
#include "ByteBuffer.hpp"

class WorldPacket : public ByteBuffer
{
    public:
        WorldPacket() : ByteBuffer(0), m_opcode(NULL_OPCODE) { }

        WorldPacket(Opcodes opcode, size_t res = 20) : ByteBuffer(res), m_opcode(opcode) { }

        WorldPacket(WorldPacket const& packet) : ByteBuffer(packet), m_opcode(packet.m_opcode) { }

        void Initialize(Opcodes opcode, size_t newres = 20)
        {
            clear();
            _storage.reserve(newres);
            m_opcode = opcode;
        }

        Opcodes GetOpcode() const { return m_opcode; }
        void SetOpcode(Opcodes opcode) { m_opcode = opcode; }

    private:
        Opcodes m_opcode;
};

#endif

