// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2008 Gael Guennebaud <g.gael@free.fr>
// Copyright (C) 2008 Benoit Jacob <jacob.benoit.1@gmail.com>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#include "main.h"
#include <Eigen/LU>

template<typename MatrixType> void inverse(const MatrixType& m)
{
  /* this test covers the following files:
     Inverse.h
  */
  int rows = m.rows();
  int cols = m.cols();

  typedef typename MatrixType::Scalar Scalar;
  typedef typename NumTraits<Scalar>::Real RealScalar;
  typedef Matrix<Scalar, MatrixType::ColsAtCompileTime, 1> VectorType;

  MatrixType m1 = MatrixType::Random(rows, cols),
             m2(rows, cols),
             mzero = MatrixType::Zero(rows, cols),
             identity = MatrixType::Identity(rows, rows);

  if (ei_is_same_type<RealScalar,float>::ret)
  {
    // let's build a more stable to inverse matrix
    MatrixType a = MatrixType::Random(rows,cols);
    m1 += m1 * m1.adjoint() + a * a.adjoint();
  }

  m2 = m1.inverse();
  VERIFY_IS_APPROX(m1, m2.inverse() );

  m1.computeInverse(&m2);
  VERIFY_IS_APPROX(m1, m2.inverse() );

  VERIFY_IS_APPROX((Scalar(2)*m2).inverse(), m2.inverse()*Scalar(0.5));

  VERIFY_IS_APPROX(identity, m1.inverse() * m1 );
  VERIFY_IS_APPROX(identity, m1 * m1.inverse() );

  VERIFY_IS_APPROX(m1, m1.inverse().inverse() );

  // since for the general case we implement separately row-major and col-major, test that
  VERIFY_IS_APPROX(m1.transpose().inverse(), m1.inverse().transpose());
}

void test_inverse()
{
  for(int i = 0; i < g_repeat; i++) {
    CALL_SUBTEST( inverse(Matrix<double,1,1>()) );
    CALL_SUBTEST( inverse(Matrix2d()) );
    CALL_SUBTEST( inverse(Matrix3f()) );
    CALL_SUBTEST( inverse(Matrix4f()) );
    CALL_SUBTEST( inverse(MatrixXf(8,8)) );
    CALL_SUBTEST( inverse(MatrixXcd(7,7)) );
  }
  
  // test some tricky cases for 4x4 matrices
  VERIFY_IS_APPROX((Matrix4f() << 0,0,1,0, 1,0,0,0, 0,1,0,0, 0,0,0,1).finished().inverse(),
                   (Matrix4f() << 0,1,0,0, 0,0,1,0, 1,0,0,0, 0,0,0,1).finished());
  VERIFY_IS_APPROX((Matrix4f() << 1,0,0,0, 0,0,1,0, 0,0,0,1, 0,1,0,0).finished().inverse(),
                   (Matrix4f() << 1,0,0,0, 0,0,0,1, 0,1,0,0, 0,0,1,0).finished());
}
