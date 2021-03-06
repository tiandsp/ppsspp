// Copyright (c) 2012- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.

#pragma once

#include <list>
#include <vector>

#include "../GPUInterface.h"

#include "gfx_es2/fbo.h"

class ShaderManager;

class GLES_GPU : public GPUInterface
{
public:
	GLES_GPU(int renderWidth, int renderHeight);
	~GLES_GPU();
	virtual void InitClear();
	virtual u32 EnqueueList(u32 listpc, u32 stall);
	virtual void UpdateStall(int listid, u32 newstall);
	virtual void ExecuteOp(u32 op, u32 diff);
	virtual bool InterpretList();
	virtual void DrawSync(int mode);
	virtual void EnableInterrupts(bool enable) {
		interruptsEnabled_ = enable;
	}

	virtual void SetDisplayFramebuffer(u32 framebuf, u32 stride, int format);
	virtual void CopyDisplayToOutput();
	virtual void BeginFrame();

private:

	bool ProcessDLQueue();
	bool interruptsEnabled_;

	u32 displayFramebufPtr_;
	u32 displayStride_;
	int displayFormat_;

	int renderWidth_;
	int renderHeight_;

	float widthFactor_;
	float heightFactor_;

	struct CmdProcessorState
	{
		u32 pc;
		u32 stallAddr;
	};

	CmdProcessorState dcontext;

	int dlIdGenerator;

	struct DisplayList
	{
		int id;
		u32 listpc;
		u32 stall;
	};

	std::vector<DisplayList> dlQueue;

	u32 prev;
	u32 stack[2];
	u32 stackptr;
	bool finished;

	struct VirtualFramebuffer {
		u32 fb_address;
		u32 z_address;
		int fb_stride;
		int z_stride;

		// There's also a top left of the drawing region, but meh...
		int width;
		int height;

		int format;  // virtual, right now they are all RGBA8888
		FBO *fbo;
	};

	void SetRenderFrameBuffer();  // Uses parameters computed from gstate
	// TODO: Break out into some form of FBO manager
	VirtualFramebuffer *GetDisplayFBO();

	std::list<VirtualFramebuffer *> vfbs_;

	VirtualFramebuffer *currentRenderVfb_;

	u8 bezierBuf[16000];
};
