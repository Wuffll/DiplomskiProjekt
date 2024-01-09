#include "Spline.h"

#include "Debug.h"

CubicBSpline::CubicBSpline(std::vector<glm::vec3>& controlPoints, const unsigned int& sampleRate)
	:
	mControlPoints(controlPoints),
	mSampleRate(sampleRate),
	mNumOfSegments(controlPoints.size() - 3)
{
	FillSplinePoints(controlPoints, sampleRate);
}

CubicBSpline::~CubicBSpline()
{
}

const std::vector<Vertex>& CubicBSpline::GetSplinePoints() const
{
	return mSplinePoints;
}

const std::vector<glm::mat4>& CubicBSpline::GetRotationMatrices() const
{
	return mRotationMatrices;
}

void CubicBSpline::FillSplinePoints(std::vector<glm::vec3>& controlPoints, const unsigned int& sampleRate)
{
	mSplinePoints.reserve(sampleRate + 6);
	mTangents.reserve(sampleRate);
	mNormals.reserve(sampleRate);
	mBinormals.reserve(sampleRate);
	mRotationMatrices.reserve(sampleRate);
	mCurrentGuides.reserve(6);

	if (controlPoints.size() < 4)
		Debug::ThrowException("Must have at least 4 control points! (current size = " + STRING(controlPoints.size()) + ")");

	std::vector<unsigned int> indices;
	indices.reserve(sampleRate);

	int numPointsPerSegment = sampleRate / mNumOfSegments;
	float delta = 1.0f / (numPointsPerSegment - 1);

	glm::mat3 rotationMatrix(1.0f);

	int l = 0;
	for (size_t i = 1; i <= controlPoints.size() - 3; i++)
	{
		glm::vec4 T{};
		glm::vec3 T2{};
		glm::vec2 T3{};

		glm::mat4x3 R{ controlPoints[i - 1], controlPoints[i], controlPoints[i + 1], controlPoints[i + 2]};

		glm::mat4 B{ {-1.0f, 3.0f, -3.0f, 1.0f}, {3.0f, -6.0f, 3.0f, 0.0f}, {-3.0f, 0.0f, 3.0f, 0.0f}, {1.0f, 4.0f, 1.0f, 0.0} };
		glm::mat3x4 B2{ {-1.0f, 3.0f, -3.0f, 1.0f}, {2.0f, -4.0f, 2.0f, 0.0f}, {-1.0f, 0.0f, 1.0f, 0.0f} };
		glm::mat2x4 B3{ {-1.0f, 3.0f, -3.0f, 1.0f}, {1.0f, -2.0f, 1.0f, 0.0f} };

		glm::vec3 opResult, tangResult, tang2Result;
		for (float j = 0.0f; j < 1.0f; j += delta)
		{
			T = {j*j*j, j*j, j, 1.0f};
			T2 = { j*j, j, 1.0f };
			T3 = { j, 1.0f };

			opResult = { R * B * (T * (1.0f / 6.0f)) }; // spline points
			tangResult = { R * B2 * (T2 * (1.0f / 2.0f)) }; // first derivative
			tang2Result = { R * B3 * T3 }; // second derivative

			mSplinePoints.push_back({ opResult, {1.0f, 1.0f, 1.0f} });

			mTangents.push_back({glm::normalize(tangResult), {1.0f, -1.0f, -1.0f}});
			mNormals.push_back({ glm::normalize(glm::cross(mTangents.back().pos, (tang2Result))), {-1.0f, 1.0f, -1.0f} });
			mBinormals.push_back({ glm::normalize(glm::cross(mNormals.back().pos, mTangents.back().pos)), {-1.0f, -1.0f, 1.0f} });


			rotationMatrix = { mBinormals.back().pos, mNormals.back().pos, mTangents.back().pos };

			// rotationMatrix = glm::inverse(rotationMatrix);

			mRotationMatrices.push_back(rotationMatrix);

			indices.push_back(l);
			l++;
		}
	}

	mSplinePoints.push_back(mSplinePoints[0]);
	mSplinePoints.push_back(mSplinePoints[0].AddPosition(mTangents[0]));
	mSplinePoints.push_back(mSplinePoints[0]);
	mSplinePoints.push_back(mSplinePoints[0].AddPosition(mNormals[0]));
	mSplinePoints.push_back(mSplinePoints[0]);
	mSplinePoints.push_back(mSplinePoints[0].AddPosition(mBinormals[0]));

	mActive++;

	mVArray.Bind();

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);

	mVArray.SetLayout(layout, false);
	mVArray.SetDrawingMode(GL_LINE_STRIP);
	mVArray.SetUsage(GL_STATIC_DRAW);

	mVBuffer.FillBuffer(mSplinePoints.data(), mSplinePoints.size() * sizeof(Vertex), mVArray.GetUsage());
	mIBuffer.FillBuffer(indices.data(), indices.size(), GL_STATIC_DRAW);

	mVArray.AddBuffer(mVBuffer, mIBuffer);
}

void CubicBSpline::Draw()
{
	mVArray.Bind();
	mVBuffer.Bind<Vertex>(0);
	mIBuffer.Bind(); // i thought that VAO stored state about the index buffer ???

	glDrawElements(mVArray.GetDrawingMode(), mSplinePoints.size() - 6, GL_UNSIGNED_INT, nullptr);
	
	glLineWidth(3.0f);
	glDrawArrays(GL_LINES, mSplinePoints.size() - 6, 6);
	glLineWidth(1.0f);

	auto size = mSplinePoints.size();
	mSplinePoints[size - 6] = (mSplinePoints[mActive]);
	mSplinePoints[size - 5] = (mSplinePoints[mActive].AddPosition(mTangents[mActive]));
	mSplinePoints[size - 4] = (mSplinePoints[mActive]);
	mSplinePoints[size - 3] = (mSplinePoints[mActive].AddPosition(mNormals[mActive]));
	mSplinePoints[size - 2] = (mSplinePoints[mActive]);
	mSplinePoints[size - 1] = (mSplinePoints[mActive].AddPosition(mBinormals[mActive]));

	mActive++;

	if ((size_t)mActive >= mTangents.size())
		mActive = 0;

	mVBuffer.FillBuffer(mSplinePoints.data(), mSplinePoints.size() * sizeof(Vertex), mVArray.GetUsage());
}

const bool& CubicBSpline::IsActive() const
{
	return mBoolActive;
}

void CubicBSpline::SetActive(const bool& value)
{
	mBoolActive = value;
}

Transform& CubicBSpline::GetTransform()
{
	return mTransform;
}

const std::vector<Vertex>& CubicBSpline::GetTangents() const
{
	return mTangents;
}
