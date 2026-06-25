using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel.DataAnnotations;

namespace WarframeMasteryChecklist.Models
{
    public enum ItemType
    {
        [Display(Name = "Primary Weapons")]     PrimaryWeapon,
        [Display(Name = "Secondary Weapons")]   SecondaryWeapon,
        [Display(Name = "Melee Weapons")]       MeleeWeapon,
        [Display(Name = "Warframes")]           Warframe,
        [Display(Name = "Companions")]          Companion
    }
}
