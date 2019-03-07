using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIManager : MonoBehaviour
{
    [SerializeField]
    private Text ammoText;
    [SerializeField]
    private GameObject coin;
    [SerializeField]
    private GameObject reloadingGO; // new addition

    public void updateAmmo(int count)
    {
        ammoText.text = "Ammo: " + count;
    }

    public void CollectedCoin()
    {
        coin.SetActive(true);
    }

    public void RemoveCoin()
    {
        coin.SetActive(false);
    }

    public void ReloadingOn()
    {
        reloadingGO.SetActive(true);
    }

    public void ReloadingOff()
    {
        reloadingGO.SetActive(false);
    }
}
