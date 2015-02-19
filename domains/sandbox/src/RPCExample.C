/*
 * Copyright (C) 2012 by
 *   MetraLabs GmbH (MLAB), GERMANY
 * and
 *   Neuroinformatics and Cognitive Robotics Labs (NICR) at TU Ilmenau, GERMANY
 * All rights reserved.
 *
 * Contact: info@mira-project.org
 *
 * Commercial Usage:
 *   Licensees holding valid commercial licenses may use this file in
 *   accordance with the commercial license agreement provided with the
 *   software or, alternatively, in accordance with the terms contained in
 *   a written agreement between you and MLAB or NICR.
 *
 * GNU General Public License Usage:
 *   Alternatively, this file may be used under the terms of the GNU
 *   General Public License version 3.0 as published by the Free Software
 *   Foundation and appearing in the file LICENSE.GPL3 included in the
 *   packaging of this file. Please review the following information to
 *   ensure the GNU General Public License version 3.0 requirements will be
 *   met: http://www.gnu.org/copyleft/gpl.html.
 *   Alternatively you may (at your option) use any later version of the GNU
 *   General Public License if such license has been publicly approved by
 *   MLAB and NICR (or its successors, if any).
 *
 * IN NO EVENT SHALL "MLAB" OR "NICR" BE LIABLE TO ANY PARTY FOR DIRECT,
 * INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF
 * THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF "MLAB" OR
 * "NICR" HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * "MLAB" AND "NICR" SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND "MLAB" AND "NICR" HAVE NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS OR MODIFICATIONS.
 */

/**
 * @file RPCExample.C
 *    This unit gives a small example on how to use RPCs to move the RoboHead.
 *
 * @author Dirk Steindorf
 * @date   2015/02/19
 */

#include <fw/MicroUnit.h>
#include <cstdlib>

using namespace mira;

namespace sandbox { 

///////////////////////////////////////////////////////////////////////////////

/**
 * This unit gives a small example on how to use RPCs to move the RoboHead.
 */
class RPCExample : public MicroUnit
{
MIRA_OBJECT(RPCExample)

public:

	RPCExample();

	template<typename Reflector>
	void reflect(Reflector& r)
	{
		MicroUnit::reflect(r);

		r.method("nod", &RPCExample::nod, this, "This method lets the RoboHead nod.");
	}

protected:

	virtual void initialize();

private:
	void nod();

private:
	float headUp;
	float headDown;
};

///////////////////////////////////////////////////////////////////////////////

RPCExample::RPCExample()
{
}

void RPCExample::initialize()
{
	headUp = 0.0f;
	headDown = -4.0f;
	publishService(*this);
}

void RPCExample::nod()
{
	std::string headProperty = "Head.HeadTilt";
	std::string value;
	RPCFuture<std::string> result;

	waitForService("/robot/Robot");
	callService<void>("/robot/Robot", "moveHeadUpDown", headDown);

	do
	{
		result = callService<std::string>("/robot/Robot#builtin", "getProperty", headProperty);
		result.wait();
		value = result.get();
	}
	while(atof(value.c_str()) > headDown + 0.3f);

	callService<void>("/robot/Robot", "moveHeadUpDown", headUp);
}

///////////////////////////////////////////////////////////////////////////////

}

MIRA_CLASS_SERIALIZATION(sandbox::RPCExample, mira::MicroUnit );
