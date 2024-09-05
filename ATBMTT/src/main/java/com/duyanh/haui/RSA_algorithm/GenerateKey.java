package com.duyanh.haui.RSA_algorithm;

import java.math.BigInteger;


public class GenerateKey {
    
    private BigInteger component;
    private BigInteger modulus;

    public GenerateKey(BigInteger component, BigInteger modulus) {
        this.component = component;
        this.modulus = modulus;
    }

    public BigInteger getComponent() {
        return component;
    }

    public void setComponent(BigInteger component) {
        this.component = component;
    }

    public BigInteger getModulus() {
        return modulus;
    }

    public void setModulus(BigInteger modulus) {
        this.modulus = modulus;
    }
}

