#include "BlocksEngine/pch.h"

#include "BlocksEngine/Core/Math/Matrix3x3.h"

using namespace BlocksEngine;
using namespace DirectX;

//------------------------------------------------------------------------------
// Constructors, Destructors & Copy, Move
//------------------------------------------------------------------------------

Matrix3X3::Matrix3X3() noexcept
    : XMFLOAT3X3{
        1.0f, 0, 0,
        0, 1.0f, 0,
        0, 0, 1.0f
    }
{
}

constexpr Matrix3X3::Matrix3X3(const float m00, const float m01, const float m02,
                               const float m10, const float m11, const float m12,
                               const float m20, const float m21, const float m22) noexcept
    : XMFLOAT3X3{
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22
    }
{
}

Matrix3X3::Matrix3X3(const Vector3<float>& r0, const Vector3<float>& r1, const Vector3<float>& r2) noexcept
    : XMFLOAT3X3{
        r0.x, r0.y, r0.z,
        r1.x, r1.y, r1.z,
        r2.x, r2.y, r2.z,
    }
{
}

Matrix3X3::Matrix3X3(const XMFLOAT3X3& m) noexcept
    : XMFLOAT3X3{}
{
    memcpy(this, &m, sizeof XMFLOAT3X3);
}

Matrix3X3::Matrix3X3(_In_reads_(9) const float* pArray) noexcept
    : XMFLOAT3X3(pArray)
{
}

Matrix3X3::Matrix3X3(CXMMATRIX m) noexcept
    : XMFLOAT3X3{}
{
    XMStoreFloat3x3(this, m);
}

//------------------------------------------------------------------------------
// Operators
//------------------------------------------------------------------------------

Matrix3X3::operator DirectX::XMMATRIX() const noexcept
{
    return XMLoadFloat3x3(this);
}

Vector3<float> Matrix3X3::operator[](const int index) const
{
    switch (index)
    {
    case 0: return XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_11));
    case 1: return XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_21));
    case 2: return XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_31));
    default: throw std::out_of_range("The index " + std::to_string(index) + " is out of range");
    }
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

bool Matrix3X3::operator==(const Matrix3X3& m) const noexcept
{
    const XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_11));
    const XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_21));
    const XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_31));

    const XMVECTOR y1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._11));
    const XMVECTOR y2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._21));
    const XMVECTOR y3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._31));

    return XMVector3Equal(x1, y1)
        && XMVector3Equal(x2, y2)
        && XMVector3Equal(x3, y3);
}

bool Matrix3X3::operator!=(const Matrix3X3& m) const noexcept
{
    const XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_11));
    const XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_21));
    const XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_31));

    const XMVECTOR y1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._11));
    const XMVECTOR y2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._21));
    const XMVECTOR y3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._31));

    return XMVector3NotEqual(x1, y1)
        || XMVector3NotEqual(x2, y2)
        || XMVector3NotEqual(x3, y3);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

Matrix3X3& Matrix3X3::operator+=(const Matrix3X3& m) noexcept
{
    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_31));

    const XMVECTOR y1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._11));
    const XMVECTOR y2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._21));
    const XMVECTOR y3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._31));

    x1 = XMVectorAdd(x1, y1);
    x2 = XMVectorAdd(x2, y2);
    x3 = XMVectorAdd(x3, y3);

    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_31), x3);

    return *this;
}

Matrix3X3& Matrix3X3::operator-=(const Matrix3X3& m) noexcept
{
    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_31));

    const XMVECTOR y1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._11));
    const XMVECTOR y2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._21));
    const XMVECTOR y3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._31));

    x1 = XMVectorSubtract(x1, y1);
    x2 = XMVectorSubtract(x2, y2);
    x3 = XMVectorSubtract(x3, y3);

    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_31), x3);

    return *this;
}

Matrix3X3& Matrix3X3::operator*=(const Matrix3X3& m) noexcept
{
    const XMMATRIX m1 = XMLoadFloat3x3(this);
    const XMMATRIX m2 = XMLoadFloat3x3(&m);
    const XMMATRIX x = XMMatrixMultiply(m1, m2);

    XMStoreFloat3x3(this, x);

    return *this;
}

Matrix3X3& Matrix3X3::operator*=(const float s) noexcept
{
    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_31));

    x1 = XMVectorScale(x1, s);
    x2 = XMVectorScale(x2, s);
    x3 = XMVectorScale(x3, s);

    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_31), x3);

    return *this;
}

Matrix3X3& Matrix3X3::operator/=(const float s) noexcept
{
    assert(s != 0.f);
    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_31));

    const float rs = 1.f / s;

    x1 = XMVectorScale(x1, rs);
    x2 = XMVectorScale(x2, rs);
    x3 = XMVectorScale(x3, rs);

    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_31), x3);

    return *this;
}

Matrix3X3& Matrix3X3::operator/=(const Matrix3X3& m) noexcept
{
    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_31));

    const XMVECTOR y1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._11));
    const XMVECTOR y2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._21));
    const XMVECTOR y3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._31));

    x1 = XMVectorDivide(x1, y1);
    x2 = XMVectorDivide(x2, y2);
    x3 = XMVectorDivide(x3, y3);

    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&_31), x3);

    return *this;
}

//------------------------------------------------------------------------------
// Unary operators
//------------------------------------------------------------------------------

Matrix3X3 Matrix3X3::operator+() const noexcept
{
    return *this;
}

Matrix3X3 Matrix3X3::operator-() const noexcept
{
    XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_11));
    XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_21));
    XMVECTOR v3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&_31));

    v1 = XMVectorNegate(v1);
    v2 = XMVectorNegate(v2);
    v3 = XMVectorNegate(v3);

    Matrix3X3 r;
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._11), v1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._21), v2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._31), v3);
    return r;
}

//------------------------------------------------------------------------------
// Properties
//------------------------------------------------------------------------------

Vector3<float> Matrix3X3::Up() const noexcept
{
    return Vector3(_21, _22, _23);
}

void Matrix3X3::Up(const Vector3<float>& v) noexcept
{
    _21 = v.x;
    _22 = v.y;
    _23 = v.z;
}

Vector3<float> Matrix3X3::Down() const noexcept
{
    return Vector3(-_21, -_22, -_23);
}

void Matrix3X3::Down(const Vector3<float>& v) noexcept
{
    _21 = -v.x;
    _22 = -v.y;
    _23 = -v.z;
}

Vector3<float> Matrix3X3::Right() const noexcept
{
    return Vector3(_11, _12, _13);
}

void Matrix3X3::Right(const Vector3<float>& v) noexcept
{
    _11 = v.x;
    _12 = v.y;
    _13 = v.z;
}

Vector3<float> Matrix3X3::Left() const noexcept
{
    return Vector3(-_11, -_12, -_13);
}

void Matrix3X3::Left(const Vector3<float>& v) noexcept
{
    _11 = -v.x;
    _12 = -v.y;
    _13 = -v.z;
}

Vector3<float> Matrix3X3::Forward() const noexcept
{
    return Vector3(-_31, -_32, -_33);
}

void Matrix3X3::Forward(const Vector3<float>& v) noexcept
{
    _31 = -v.x;
    _32 = -v.y;
    _33 = -v.z;
}

Vector3<float> Matrix3X3::Backward() const noexcept
{
    return Vector3(_31, _32, _33);
}

void Matrix3X3::Backward(const Vector3<float>& v) noexcept
{
    _31 = v.x;
    _32 = v.y;
    _33 = v.z;
}

Vector3<float> Matrix3X3::Translation() const noexcept
{
    return Vector3(_31, _32, _33);
}

void Matrix3X3::Translation(const Vector3<float>& v) noexcept
{
    _31 = v.x;
    _32 = v.y;
    _33 = v.z;
}

//------------------------------------------------------------------------------
// Matrix3X3 operations
//------------------------------------------------------------------------------

bool Matrix3X3::Decompose(Vector3<float>& scale, Quaternion& rotation, Vector3<float>& translation) const noexcept
{
    XMVECTOR s, r, t;

    if (!XMMatrixDecompose(&s, &r, &t, *this))
    {
        return false;
    }

    XMStoreFloat3(&scale, s);
    XMStoreFloat4(&rotation, r);
    XMStoreFloat3(&translation, t);

    return true;
}

Matrix3X3 Matrix3X3::Transpose() const noexcept
{
    const XMMATRIX m = XMLoadFloat3x3(this);

    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixTranspose(m));
    return r;
}

void Matrix3X3::Transpose(Matrix3X3& result) const noexcept
{
    const XMMATRIX m = XMLoadFloat3x3(this);

    XMStoreFloat3x3(&result, XMMatrixTranspose(m));
}

Matrix3X3 Matrix3X3::Invert() const noexcept
{
    const XMMATRIX m = XMLoadFloat3x3(this);

    Matrix3X3 r;
    XMVECTOR det;
    XMStoreFloat3x3(&r, XMMatrixInverse(&det, m));
    return r;
}

void Matrix3X3::Invert(Matrix3X3& result) const noexcept
{
    const XMMATRIX m = XMLoadFloat3x3(this);

    XMVECTOR det;
    XMStoreFloat3x3(&result, XMMatrixInverse(&det, m));
}

float Matrix3X3::Determinant() const noexcept
{
    const XMMATRIX m = XMLoadFloat3x3(this);

    return XMVectorGetX(XMMatrixDeterminant(m));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

_Use_decl_annotations_

Matrix3X3 Matrix3X3::CreateBillboard(
    const Vector3<float>& object,
    const Vector3<float>& cameraPosition,
    const Vector3<float>& cameraUp,
    const Vector3<float>* cameraForward) noexcept
{
    const XMVECTOR o = XMLoadFloat3(&object);
    const XMVECTOR c = XMLoadFloat3(&cameraPosition);
    XMVECTOR z = XMVectorSubtract(o, c);

    if (const XMVECTOR n = XMVector3LengthSq(z); XMVector3Less(n, g_XMEpsilon))
    {
        if (cameraForward)
        {
            const XMVECTOR f = XMLoadFloat3(cameraForward);
            z = XMVectorNegate(f);
        }
        else
        {
            z = g_XMNegIdentityR2;
        }
    }
    else
    {
        z = XMVector3Normalize(z);
    }

    const XMVECTOR up = XMLoadFloat3(&cameraUp);
    XMVECTOR x = XMVector3Cross(up, z);
    x = XMVector3Normalize(x);

    XMVECTOR y = XMVector3Cross(z, x);

    XMMATRIX m{x, y, z, XMVectorSetW(o, 1.0f)};

    Matrix3X3 r;
    XMStoreFloat3x3(&r, m);
    return r;
}

_Use_decl_annotations_

Matrix3X3 Matrix3X3::CreateConstrainedBillboard(
    const Vector3<float>& object,
    const Vector3<float>& cameraPosition,
    const Vector3<float>& rotateAxis,
    const Vector3<float>* cameraForward,
    Vector3<float>* objectForward) noexcept
{
    // 1.0 - XMConvertToRadians( 0.1f );
    static constexpr XMVECTORF32 MinAngle = {{{0.99825467075f, 0.99825467075f, 0.99825467075f, 0.99825467075f}}};


    const XMVECTOR o = XMLoadFloat3(&object);
    const XMVECTOR c = XMLoadFloat3(&cameraPosition);
    XMVECTOR faceDir = XMVectorSubtract(o, c);

    if (const XMVECTOR n = XMVector3LengthSq(faceDir); XMVector3Less(n, g_XMEpsilon))
    {
        if (cameraForward)
        {
            const XMVECTOR f = XMLoadFloat3(cameraForward);
            faceDir = XMVectorNegate(f);
        }
        else
        {
            faceDir = g_XMNegIdentityR2;
        }
    }
    else
    {
        faceDir = XMVector3Normalize(faceDir);
    }

    const XMVECTOR y = XMLoadFloat3(&rotateAxis);
    XMVECTOR x;
    XMVECTOR z;

    if (XMVECTOR dot = XMVectorAbs(XMVector3Dot(y, faceDir)); XMVector3Greater(dot, MinAngle))
    {
        if (objectForward)
        {
            z = XMLoadFloat3(objectForward);
            dot = XMVectorAbs(XMVector3Dot(y, z));
            if (XMVector3Greater(dot, MinAngle))
            {
                dot = XMVectorAbs(XMVector3Dot(y, g_XMNegIdentityR2));
                z = (XMVector3Greater(dot, MinAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
            }
        }
        else
        {
            dot = XMVectorAbs(XMVector3Dot(y, g_XMNegIdentityR2));
            z = (XMVector3Greater(dot, MinAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
        }

        x = XMVector3Cross(y, z);
        x = XMVector3Normalize(x);

        z = XMVector3Cross(x, y);
        z = XMVector3Normalize(z);
    }
    else
    {
        x = XMVector3Cross(y, faceDir);
        x = XMVector3Normalize(x);

        z = XMVector3Cross(x, y);
        z = XMVector3Normalize(z);
    }

    const XMMATRIX m{x, y, z, XMVectorSetW(o, 1.0f)};

    Matrix3X3 r;
    XMStoreFloat3x3(&r, m);
    return r;
}

Matrix3X3 Matrix3X3::CreateTranslation(const Vector3<float>& position) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixTranslation(position.x, position.y, position.z));
    return r;
}

Matrix3X3 Matrix3X3::CreateTranslation(const float x, const float y, const float z) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixTranslation(x, y, z));
    return r;
}

Matrix3X3 Matrix3X3::CreateScale(const Vector3<float>& scales) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixScaling(scales.x, scales.y, scales.z));
    return r;
}

Matrix3X3 Matrix3X3::CreateScale(const float xs, const float ys, const float zs) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixScaling(xs, ys, zs));
    return r;
}

Matrix3X3 Matrix3X3::CreateScale(const float scale) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixScaling(scale, scale, scale));
    return r;
}

Matrix3X3 Matrix3X3::CreateRotationX(const float radians) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixRotationX(radians));
    return r;
}

Matrix3X3 Matrix3X3::CreateRotationY(const float radians) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixRotationY(radians));
    return r;
}

Matrix3X3 Matrix3X3::CreateRotationZ(const float radians) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixRotationZ(radians));
    return r;
}

Matrix3X3 Matrix3X3::CreateFromAxisAngle(const Vector3<float>& axis, const float angle) noexcept
{
    Matrix3X3 r;
    const XMVECTOR a = XMLoadFloat3(&axis);
    XMStoreFloat3x3(&r, XMMatrixRotationAxis(a, angle));
    return r;
}

Matrix3X3 Matrix3X3::CreatePerspectiveFieldOfView(const float fov, const float aspectRatio, const float nearPlane,
                                                  const float farPlane) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
    return r;
}

Matrix3X3 Matrix3X3::CreatePerspective(const float width, const float height, const float nearPlane,
                                       const float farPlane) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixPerspectiveLH(width, height, nearPlane, farPlane));
    return r;
}

Matrix3X3 Matrix3X3::CreatePerspectiveOffCenter(const float left, const float right, const float bottom,
                                                const float top,
                                                const float nearPlane, const float farPlane) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixPerspectiveOffCenterLH(left, right, bottom, top, nearPlane, farPlane));
    return r;
}

Matrix3X3 Matrix3X3::CreateOrthographic(const float width, const float height, const float zNearPlane,
                                        const float zFarPlane) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixOrthographicLH(width, height, zNearPlane, zFarPlane));
    return r;
}

Matrix3X3 Matrix3X3::CreateOrthographicOffCenter(const float left, const float right, const float bottom,
                                                 const float top,
                                                 const float zNearPlane, const float zFarPlane) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixOrthographicOffCenterLH(left, right, bottom, top, zNearPlane, zFarPlane));
    return r;
}

Matrix3X3 Matrix3X3::CreateLookAt(const Vector3<float>& position, const Vector3<float>& target,
                                  const Vector3<float>& up) noexcept
{
    Matrix3X3 r;
    const XMVECTOR eyeV = XMLoadFloat3(&position);
    const XMVECTOR targetV = XMLoadFloat3(&target);
    const XMVECTOR upV = XMLoadFloat3(&up);
    XMStoreFloat3x3(&r, XMMatrixLookAtLH(eyeV, targetV, upV));
    return r;
}

Matrix3X3 Matrix3X3::CreateFromQuaternion(const Quaternion& quat) noexcept
{
    Matrix3X3 r;
    const XMVECTOR quatV = XMLoadFloat4(&quat);
    XMStoreFloat3x3(&r, XMMatrixRotationQuaternion(quatV));
    return r;
}

Matrix3X3 Matrix3X3::CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll) noexcept
{
    Matrix3X3 r;
    XMStoreFloat3x3(&r, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
    return r;
}

void Matrix3X3::Lerp(const Matrix3X3& m1, const Matrix3X3& m2, const float t, Matrix3X3& result) noexcept
{
    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._31));

    const XMVECTOR y1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._11));
    const XMVECTOR y2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._21));
    const XMVECTOR y3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._31));

    x1 = XMVectorLerp(x1, y1, t);
    x2 = XMVectorLerp(x2, y2, t);
    x3 = XMVectorLerp(x3, y3, t);

    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result._11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result._21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result._31), x3);
}

Matrix3X3 Matrix3X3::Lerp(const Matrix3X3& m1, const Matrix3X3& m2, const float t) noexcept
{
    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._31));

    const XMVECTOR y1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._11));
    const XMVECTOR y2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._21));
    const XMVECTOR y3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._31));

    x1 = XMVectorLerp(x1, y1, t);
    x2 = XMVectorLerp(x2, y2, t);
    x3 = XMVectorLerp(x3, y3, t);

    Matrix3X3 result;
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result._11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result._21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&result._31), x3);
    return result;
}

void Matrix3X3::Transform(const Matrix3X3& m, const Quaternion& rotation, Matrix3X3& result) noexcept
{
    const XMVECTOR quatV = XMLoadFloat4(&rotation);

    const XMMATRIX m0 = XMLoadFloat3x3(&m);
    const XMMATRIX m1 = XMMatrixRotationQuaternion(quatV);

    XMStoreFloat3x3(&result, XMMatrixMultiply(m0, m1));
}

Matrix3X3 Matrix3X3::Transform(const Matrix3X3& m, const Quaternion& rotation) noexcept
{
    const XMVECTOR quatV = XMLoadFloat4(&rotation);

    const XMMATRIX m0 = XMLoadFloat3x3(&m);
    const XMMATRIX m1 = XMMatrixRotationQuaternion(quatV);

    Matrix3X3 result;
    XMStoreFloat3x3(&result, XMMatrixMultiply(m0, m1));
    return result;
}

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

const Matrix3X3 Matrix3X3::Identity = {
    1.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 1.f,
};

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

Matrix3X3 operator+(const Matrix3X3& m1, const Matrix3X3& m2) noexcept
{
    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._31));

    const XMVECTOR y1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._11));
    const XMVECTOR y2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._21));
    const XMVECTOR y3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._31));

    x1 = XMVectorAdd(x1, y1);
    x2 = XMVectorAdd(x2, y2);
    x3 = XMVectorAdd(x3, y3);

    Matrix3X3 r;
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._31), x3);
    return r;
}

Matrix3X3 operator-(const Matrix3X3& m1, const Matrix3X3& m2) noexcept
{
    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._31));

    const XMVECTOR y1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._11));
    const XMVECTOR y2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._21));
    const XMVECTOR y3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._31));

    x1 = XMVectorSubtract(x1, y1);
    x2 = XMVectorSubtract(x2, y2);
    x3 = XMVectorSubtract(x3, y3);

    Matrix3X3 r;
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._31), x3);
    return r;
}

Matrix3X3 operator*(const Matrix3X3& m1, const Matrix3X3& m2) noexcept
{
    const XMMATRIX x1 = XMLoadFloat3x3(&m1);
    const XMMATRIX x2 = XMLoadFloat3x3(&m2);
    const XMMATRIX x = XMMatrixMultiply(x1, x2);

    Matrix3X3 r;
    XMStoreFloat3x3(&r, x);
    return r;
}

Matrix3X3 operator*(const Matrix3X3& m, const float s) noexcept
{
    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._31));

    x1 = XMVectorScale(x1, s);
    x2 = XMVectorScale(x2, s);
    x3 = XMVectorScale(x3, s);

    Matrix3X3 r;
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._31), x3);
    return r;
}

Matrix3X3 operator/(const Matrix3X3& m, const float s) noexcept
{
    assert(s != 0.f);

    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._31));

    const float rs = 1.f / s;

    x1 = XMVectorScale(x1, rs);
    x2 = XMVectorScale(x2, rs);
    x3 = XMVectorScale(x3, rs);

    Matrix3X3 r;
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._31), x3);
    return r;
}

Matrix3X3 operator/(const Matrix3X3& m1, const Matrix3X3& m2) noexcept
{
    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m1._31));

    const XMVECTOR y1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._11));
    const XMVECTOR y2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._21));
    const XMVECTOR y3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m2._31));

    x1 = XMVectorDivide(x1, y1);
    x2 = XMVectorDivide(x2, y2);
    x3 = XMVectorDivide(x3, y3);

    Matrix3X3 r;
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._31), x3);
    return r;
}

Matrix3X3 operator*(const float s, const Matrix3X3& m) noexcept
{
    XMVECTOR x1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._11));
    XMVECTOR x2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._21));
    XMVECTOR x3 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m._31));

    x1 = XMVectorScale(x1, s);
    x2 = XMVectorScale(x2, s);
    x3 = XMVectorScale(x3, s);

    Matrix3X3 r;
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._11), x1);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._21), x2);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._31), x3);
    return r;
}
