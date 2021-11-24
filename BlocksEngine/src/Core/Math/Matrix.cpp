#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Math/Matrix.h"

#include <DirectXMath.h>

#include "BlocksEngine/Core/Math/Plane.h"
#include "BlocksEngine/Core/Math/Vector3.h"

using namespace BlocksEngine;
using namespace DirectX;


//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------

Matrix::Matrix() noexcept
    : XMFLOAT4X4{
        1.0f, 0, 0, 0,
        0, 1.0f, 0, 0,
        0, 0, 1.0f, 0,
        0, 0, 0, 1.0f
    }
{
}

constexpr Matrix::Matrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12,
                         float m13, float m20, float m21, float m22, float m23, float m30, float m31,
                         float m32, float m33) noexcept
    : XMFLOAT4X4{
        m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33
    }
{
}

Matrix::Matrix(const Vector3<float>& r0, const Vector3<float>& r1, const Vector3<float>& r2) noexcept
    : XMFLOAT4X4{
        r0.x, r0.y, r0.z, 0,
        r1.x, r1.y, r1.z, 0,
        r2.x, r2.y, r2.z, 0,
        0, 0, 0, 1.0f
    }
{
}

Matrix::Matrix(const Vector4<float>& r0, const Vector4<float>& r1, const Vector4<float>& r2,
               const Vector4<float>& r3) noexcept
    : XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
                 r1.x, r1.y, r1.z, r1.w,
                 r2.x, r2.y, r2.z, r2.w,
                 r3.x, r3.y, r3.z, r3.w)
{
}

Matrix::Matrix(const XMFLOAT4X4& m) noexcept
    : XMFLOAT4X4{}
{
    memcpy(this, &m, sizeof XMFLOAT4X4);
}

Matrix::Matrix(const XMFLOAT3X3& m) noexcept
    : XMFLOAT4X4{
        m._11, m._12, m._13, 0.0f,
        m._21, m._22, m._23, 0.0f,
        m._31, m._32, m._33, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }
{
}

Matrix::Matrix(const XMFLOAT4X3& m) noexcept
    : XMFLOAT4X4{
        m._11, m._12, m._13, 0.0f,
        m._21, m._22, m._23, 0.0f,
        m._31, m._32, m._33, 0.0f,
        m._41, m._42, m._43, 1.0f
    }
{
}

Matrix::Matrix(const float* pArray) noexcept
    : XMFLOAT4X4(pArray)
{
}

Matrix::Matrix(CXMMATRIX m) noexcept
    : XMFLOAT4X4{}
{
    XMStoreFloat4x4(this, m);
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

bool Matrix::operator==(const Matrix& m) const noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    const XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    const XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    const XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._11));
    const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._21));
    const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._31));
    const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._41));

    return (XMVector4Equal(x1, y1)
        && XMVector4Equal(x2, y2)
        && XMVector4Equal(x3, y3)
        && XMVector4Equal(x4, y4)) != 0;
}

bool Matrix::operator!=(const Matrix& m) const noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    const XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    const XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    const XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._11));
    const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._21));
    const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._31));
    const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._41));

    return (XMVector4NotEqual(x1, y1)
        || XMVector4NotEqual(x2, y2)
        || XMVector4NotEqual(x3, y3)
        || XMVector4NotEqual(x4, y4)) != 0;
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

Matrix& Matrix::operator=(const XMFLOAT3X3& m) noexcept
{
    _11 = m._11;
    _12 = m._12;
    _13 = m._13;
    _14 = 0.f;
    _21 = m._21;
    _22 = m._22;
    _23 = m._23;
    _24 = 0.f;
    _31 = m._31;
    _32 = m._32;
    _33 = m._33;
    _34 = 0.f;
    _41 = 0.f;
    _42 = 0.f;
    _43 = 0.f;
    _44 = 1.f;

    isDirty_ = true;

    return *this;
}

Matrix& Matrix::operator=(const XMFLOAT4X3& m) noexcept
{
    _11 = m._11;
    _12 = m._12;
    _13 = m._13;
    _14 = 0.f;
    _21 = m._21;
    _22 = m._22;
    _23 = m._23;
    _24 = 0.f;
    _31 = m._31;
    _32 = m._32;
    _33 = m._33;
    _34 = 0.f;
    _41 = m._41;
    _42 = m._42;
    _43 = m._43;
    _44 = 1.f;

    isDirty_ = true;

    return *this;
}

Matrix& Matrix::operator+=(const Matrix& m) noexcept
{
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._11));
    const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._21));
    const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._31));
    const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._41));

    x1 = XMVectorAdd(x1, y1);
    x2 = XMVectorAdd(x2, y2);
    x3 = XMVectorAdd(x3, y3);
    x4 = XMVectorAdd(x4, y4);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);

    isDirty_ = true;

    return *this;
}

Matrix& Matrix::operator-=(const Matrix& m) noexcept
{
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._11));
    const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._21));
    const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._31));
    const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._41));

    x1 = XMVectorSubtract(x1, y1);
    x2 = XMVectorSubtract(x2, y2);
    x3 = XMVectorSubtract(x3, y3);
    x4 = XMVectorSubtract(x4, y4);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);

    isDirty_ = true;

    return *this;
}

Matrix& Matrix::operator*=(const Matrix& m) noexcept
{
    const XMMATRIX m1 = XMLoadFloat4x4(this);
    const XMMATRIX m2 = XMLoadFloat4x4(&m);
    const XMMATRIX x = XMMatrixMultiply(m1, m2);

    XMStoreFloat4x4(this, x);

    isDirty_ = true;

    return *this;
}

Matrix& Matrix::operator*=(const float s) noexcept
{
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    x1 = XMVectorScale(x1, s);
    x2 = XMVectorScale(x2, s);
    x3 = XMVectorScale(x3, s);
    x4 = XMVectorScale(x4, s);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);

    isDirty_ = true;

    return *this;
}

Matrix& Matrix::operator/=(const float s) noexcept
{
    assert(s != 0.f);
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    const float rs = 1.f / s;

    x1 = XMVectorScale(x1, rs);
    x2 = XMVectorScale(x2, rs);
    x3 = XMVectorScale(x3, rs);
    x4 = XMVectorScale(x4, rs);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);

    isDirty_ = true;

    return *this;
}

Matrix& Matrix::operator/=(const Matrix& m) noexcept
{
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._11));
    const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._21));
    const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._31));
    const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._41));

    x1 = XMVectorDivide(x1, y1);
    x2 = XMVectorDivide(x2, y2);
    x3 = XMVectorDivide(x3, y3);
    x4 = XMVectorDivide(x4, y4);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);

    isDirty_ = true;

    return *this;
}

//------------------------------------------------------------------------------
// Unary operators
//------------------------------------------------------------------------------

Matrix Matrix::operator+() const noexcept
{
    return *this;
}

Matrix Matrix::operator-() const noexcept
{
    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    v1 = XMVectorNegate(v1);
    v2 = XMVectorNegate(v2);
    v3 = XMVectorNegate(v3);
    v4 = XMVectorNegate(v4);

    Matrix r;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._11), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._21), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._31), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._41), v4);
    return r;
}

//------------------------------------------------------------------------------
// State manipulation
//------------------------------------------------------------------------------

bool Matrix::IsDirty() const noexcept
{
    return isDirty_;
}

void Matrix::ConsumeIsDirty() noexcept
{
    isDirty_ = false;
}


//------------------------------------------------------------------------------
// Properties
//------------------------------------------------------------------------------

Vector3<float> Matrix::Up() const noexcept
{
    return Vector3(_21, _22, _23);
}

void Matrix::Up(const Vector3<float>& v) noexcept
{
    _21 = v.x;
    _22 = v.y;
    _23 = v.z;

    isDirty_ = true;
}

Vector3<float> Matrix::Down() const noexcept
{
    return Vector3(-_21, -_22, -_23);
}

void Matrix::Down(const Vector3<float>& v) noexcept
{
    _21 = -v.x;
    _22 = -v.y;
    _23 = -v.z;

    isDirty_ = true;
}

Vector3<float> Matrix::Right() const noexcept
{
    return Vector3(_11, _12, _13);
}

void Matrix::Right(const Vector3<float>& v) noexcept
{
    _11 = v.x;
    _12 = v.y;
    _13 = v.z;

    isDirty_ = true;
}

Vector3<float> Matrix::Left() const noexcept
{
    return Vector3(-_11, -_12, -_13);
}

void Matrix::Left(const Vector3<float>& v) noexcept
{
    _11 = -v.x;
    _12 = -v.y;
    _13 = -v.z;

    isDirty_ = true;
}

Vector3<float> Matrix::Forward() const noexcept
{
    return Vector3(-_31, -_32, -_33);
}

void Matrix::Forward(const Vector3<float>& v) noexcept
{
    _31 = -v.x;
    _32 = -v.y;
    _33 = -v.z;

    isDirty_ = true;
}

Vector3<float> Matrix::Backward() const noexcept
{
    return Vector3(_31, _32, _33);
}

void Matrix::Backward(const Vector3<float>& v) noexcept
{
    _31 = v.x;
    _32 = v.y;
    _33 = v.z;

    isDirty_ = true;
}

Vector3<float> Matrix::Translation() const noexcept
{
    return Vector3(_41, _42, _43);
}

void Matrix::Translation(const Vector3<float>& v) noexcept
{
    _41 = v.x;
    _42 = v.y;
    _43 = v.z;

    isDirty_ = true;
}

//------------------------------------------------------------------------------
// Matrix operations
//------------------------------------------------------------------------------

bool Matrix::Decompose(Vector3<float>& scale, Quaternion& rotation, Vector3<float>& translation) const noexcept
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

Matrix Matrix::Transpose() const noexcept
{
    const XMMATRIX m = XMLoadFloat4x4(this);

    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixTranspose(m));
    return r;
}

void Matrix::Transpose(Matrix& result) const noexcept
{
    const XMMATRIX m = XMLoadFloat4x4(this);

    XMStoreFloat4x4(&result, XMMatrixTranspose(m));
}

Matrix Matrix::Invert() const noexcept
{
    const XMMATRIX m = XMLoadFloat4x4(this);

    Matrix r;
    XMVECTOR det;
    XMStoreFloat4x4(&r, XMMatrixInverse(&det, m));
    return r;
}

void Matrix::Invert(Matrix& result) const noexcept
{
    const XMMATRIX m = XMLoadFloat4x4(this);

    XMVECTOR det;
    XMStoreFloat4x4(&result, XMMatrixInverse(&det, m));
}

float Matrix::Determinant() const noexcept
{
    const XMMATRIX m = XMLoadFloat4x4(this);

    return XMVectorGetX(XMMatrixDeterminant(m));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

_Use_decl_annotations_

Matrix Matrix::CreateBillboard(
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

    Matrix r;
    XMStoreFloat4x4(&r, m);
    return r;
}

_Use_decl_annotations_

Matrix Matrix::CreateConstrainedBillboard(
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

    Matrix r;
    XMStoreFloat4x4(&r, m);
    return r;
}

Matrix Matrix::CreateTranslation(const Vector3<float>& position) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixTranslation(position.x, position.y, position.z));
    return r;
}

Matrix Matrix::CreateTranslation(const float x, const float y, const float z) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixTranslation(x, y, z));
    return r;
}

Matrix Matrix::CreateScale(const Vector3<float>& scales) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixScaling(scales.x, scales.y, scales.z));
    return r;
}

Matrix Matrix::CreateScale(const float xs, const float ys, const float zs) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixScaling(xs, ys, zs));
    return r;
}

Matrix Matrix::CreateScale(const float scale) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixScaling(scale, scale, scale));
    return r;
}

Matrix Matrix::CreateRotationX(const float radians) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixRotationX(radians));
    return r;
}

Matrix Matrix::CreateRotationY(const float radians) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixRotationY(radians));
    return r;
}

Matrix Matrix::CreateRotationZ(const float radians) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixRotationZ(radians));
    return r;
}

Matrix Matrix::CreateFromAxisAngle(const Vector3<float>& axis, const float angle) noexcept
{
    Matrix r;
    const XMVECTOR a = XMLoadFloat3(&axis);
    XMStoreFloat4x4(&r, XMMatrixRotationAxis(a, angle));
    return r;
}

Matrix Matrix::CreatePerspectiveFieldOfView(const float fov, const float aspectRatio, const float nearPlane,
                                            const float farPlane) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
    return r;
}

Matrix Matrix::CreatePerspective(const float width, const float height, const float nearPlane,
                                 const float farPlane) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixPerspectiveLH(width, height, nearPlane, farPlane));
    return r;
}

Matrix Matrix::CreatePerspectiveOffCenter(const float left, const float right, const float bottom, const float top,
                                          const float nearPlane, const float farPlane) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixPerspectiveOffCenterLH(left, right, bottom, top, nearPlane, farPlane));
    return r;
}

Matrix Matrix::CreateOrthographic(const float width, const float height, const float zNearPlane,
                                  const float zFarPlane) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixOrthographicLH(width, height, zNearPlane, zFarPlane));
    return r;
}

Matrix Matrix::CreateOrthographicOffCenter(const float left, const float right, const float bottom, const float top,
                                           const float zNearPlane, const float zFarPlane) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixOrthographicOffCenterLH(left, right, bottom, top, zNearPlane, zFarPlane));
    return r;
}

Matrix Matrix::CreateLookAt(const Vector3<float>& position, const Vector3<float>& target,
                            const Vector3<float>& up) noexcept
{
    Matrix r;
    const XMVECTOR eyeV = XMLoadFloat3(&position);
    const XMVECTOR targetV = XMLoadFloat3(&target);
    const XMVECTOR upV = XMLoadFloat3(&up);
    XMStoreFloat4x4(&r, XMMatrixLookAtLH(eyeV, targetV, upV));
    return r;
}

Matrix Matrix::CreateWorld(const Vector3<float>& position, const Vector3<float>& forward,
                           const Vector3<float>& up) noexcept
{
    const XMVECTOR zAxis = XMVector3Normalize(XMVectorNegate(XMLoadFloat3(&forward)));
    XMVECTOR yAxis = XMLoadFloat3(&up);
    const XMVECTOR xAxis = XMVector3Normalize(XMVector3Cross(yAxis, zAxis));
    yAxis = XMVector3Cross(zAxis, xAxis);

    Matrix r;
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._11), xAxis);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._21), yAxis);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&r._31), zAxis);
    r._14 = r._24 = r._34 = 0.f;
    r._41 = position.x;
    r._42 = position.y;
    r._43 = position.z;
    r._44 = 1.f;
    return r;
}

Matrix Matrix::CreateFromQuaternion(const Quaternion& quat) noexcept
{
    Matrix r;
    const XMVECTOR quatV = XMLoadFloat4(&quat);
    XMStoreFloat4x4(&r, XMMatrixRotationQuaternion(quatV));
    return r;
}

Matrix Matrix::CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll) noexcept
{
    Matrix r;
    XMStoreFloat4x4(&r, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
    return r;
}

Matrix Matrix::CreateShadow(const Vector3<float>& lightDir, const Plane& plane) noexcept
{
    Matrix r;
    const XMVECTOR light = XMLoadFloat3(&lightDir);
    const XMVECTOR planeV = XMLoadFloat4(&plane);
    XMStoreFloat4x4(&r, XMMatrixShadow(planeV, light));
    return r;
}

Matrix Matrix::CreateReflection(const Plane& plane) noexcept
{
    Matrix r;
    const XMVECTOR planeV = XMLoadFloat4(&plane);
    XMStoreFloat4x4(&r, XMMatrixReflect(planeV));
    return r;
}

void Matrix::Lerp(const Matrix& m1, const Matrix& m2, float t, Matrix& result) noexcept
{
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._41));

    x1 = XMVectorLerp(x1, y1, t);
    x2 = XMVectorLerp(x2, y2, t);
    x3 = XMVectorLerp(x3, y3, t);
    x4 = XMVectorLerp(x4, y4, t);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
}

Matrix Matrix::Lerp(const Matrix& m1, const Matrix& m2, float t) noexcept
{
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._41));

    x1 = XMVectorLerp(x1, y1, t);
    x2 = XMVectorLerp(x2, y2, t);
    x3 = XMVectorLerp(x3, y3, t);
    x4 = XMVectorLerp(x4, y4, t);

    Matrix result;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
    return result;
}

void Matrix::Transform(const Matrix& m, const Quaternion& rotation, Matrix& result) noexcept
{
    const XMVECTOR quatV = XMLoadFloat4(&rotation);

    const XMMATRIX m0 = XMLoadFloat4x4(&m);
    const XMMATRIX m1 = XMMatrixRotationQuaternion(quatV);

    XMStoreFloat4x4(&result, XMMatrixMultiply(m0, m1));
}

Matrix Matrix::Transform(const Matrix& m, const Quaternion& rotation) noexcept
{
    const XMVECTOR quatV = XMLoadFloat4(&rotation);

    const XMMATRIX m0 = XMLoadFloat4x4(&m);
    const XMMATRIX m1 = XMMatrixRotationQuaternion(quatV);

    Matrix result;
    XMStoreFloat4x4(&result, XMMatrixMultiply(m0, m1));
    return result;
}

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

const Matrix Matrix::Identity = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
};

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

Matrix operator+(const Matrix& m1, const Matrix& m2) noexcept
{
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._41));

    x1 = XMVectorAdd(x1, y1);
    x2 = XMVectorAdd(x2, y2);
    x3 = XMVectorAdd(x3, y3);
    x4 = XMVectorAdd(x4, y4);

    Matrix r;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._41), x4);
    return r;
}

Matrix operator-(const Matrix& m1, const Matrix& m2) noexcept
{
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._41));

    x1 = XMVectorSubtract(x1, y1);
    x2 = XMVectorSubtract(x2, y2);
    x3 = XMVectorSubtract(x3, y3);
    x4 = XMVectorSubtract(x4, y4);

    Matrix r;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._41), x4);
    return r;
}

Matrix operator*(const Matrix& m1, const Matrix& m2) noexcept
{
    const XMMATRIX x1 = XMLoadFloat4x4(&m1);
    const XMMATRIX x2 = XMLoadFloat4x4(&m2);
    const XMMATRIX x = XMMatrixMultiply(x1, x2);

    Matrix r;
    XMStoreFloat4x4(&r, x);
    return r;
}

Matrix operator*(const Matrix& m, const float s) noexcept
{
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._41));

    x1 = XMVectorScale(x1, s);
    x2 = XMVectorScale(x2, s);
    x3 = XMVectorScale(x3, s);
    x4 = XMVectorScale(x4, s);

    Matrix r;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._41), x4);
    return r;
}

Matrix operator/(const Matrix& m, const float s) noexcept
{
    assert(s != 0.f);

    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._41));

    const float rs = 1.f / s;

    x1 = XMVectorScale(x1, rs);
    x2 = XMVectorScale(x2, rs);
    x3 = XMVectorScale(x3, rs);
    x4 = XMVectorScale(x4, rs);

    Matrix r;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._41), x4);
    return r;
}

Matrix operator/(const Matrix& m1, const Matrix& m2) noexcept
{
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1._41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2._41));

    x1 = XMVectorDivide(x1, y1);
    x2 = XMVectorDivide(x2, y2);
    x3 = XMVectorDivide(x3, y3);
    x4 = XMVectorDivide(x4, y4);

    Matrix r;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._41), x4);
    return r;
}

Matrix operator*(const float s, const Matrix& m) noexcept
{
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m._41));

    x1 = XMVectorScale(x1, s);
    x2 = XMVectorScale(x2, s);
    x3 = XMVectorScale(x3, s);
    x4 = XMVectorScale(x4, s);

    Matrix r;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&r._41), x4);
    return r;
}
