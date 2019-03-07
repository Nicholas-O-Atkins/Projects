using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIManager : MonoBehaviour
{
    public Sprite[] lives;
    public Image livesImageDisplay;
    public GameObject titleScreen;
    public GameObject instruct; // For the intructions
    private PitchControl pitcher;

    public Text scoretext;
    public int score;
    public Text levelText;
    public int level = 0;

    public void Start()
    {
        pitcher = GameObject.Find("Main Camera").GetComponent<PitchControl>();
    }

    public void UpdateLives(int health)
    {
        livesImageDisplay.sprite = lives[health];
    }

    public void UpdateScore()
    {
        score += 10;

        scoretext.text = "Score: " + score;

        if(score >= 100) //This can be changed to accomadate a nextlevel int that hold the next score and get updated when you reach it to change to dyanmic level increases
        {
            UpdateLevel();
        }
    }

    public void ShowTitle()
    {
        titleScreen.SetActive(true);
        instruct.SetActive(true); // Adds with the title
    }

    public void HideTitle()
    {
        titleScreen.SetActive(false);
        instruct.SetActive(false); //Added for UX Purposes
        score = 0;
        scoretext.text = "Score: 0";

        level = 0;
        levelText.text = "Level: 0";

        pitcher.ResetPitch(); //set pitch back to 1 when you start
    }
    //Changes the level based on the score
    public void UpdateLevel()
    {
        level = score / 100;

        levelText.text = "Level: " + level;

        pitcher.UpdatePitch(); //Signal to the PitchControl to update the music pitch
    }
}
